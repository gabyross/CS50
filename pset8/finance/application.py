import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():

    """Show portfolio of stocks"""

    # Table summarizing, for the user currently logged in
    rows = db.execute("""
    SELECT symbol, SUM(shares) AS totalShares
    FROM transactions
    WHERE user_id = :user_id
    GROUP BY symbol
    HAVING totalShares > 0;
    """, user_id = session["user_id"])

    holdings = []
    grand_total = 0

    # Stock the user owns, the numbers of shares owned, the current price of each stock, and the total value of each holding
    for row in rows:
        print(row)
        stock = lookup(row['symbol'])

        holdings.append({
            "symbol": stock['symbol'],
            "name": stock['name'],
            "shares": row['totalShares'],
            "price": usd(stock['price']),
            "total": usd(stock['price'] * row['totalShares'])
        })

        grand_total += (stock["price"] * row['totalShares'])

    rows = db.execute("SELECT cash FROM users WHERE id=:user_id", user_id=session["user_id"])

    cash = rows[0]["cash"]

    # User’s current cash balance along with a grand total
    grand_total += cash

    return render_template("index.html", holdings=holdings, cash=usd(cash), grand_total=usd(grand_total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    """Buy shares of stock"""

    if request.method == "POST":

        shares = float(request.form.get("shares"))
        symbol = request.form.get("symbol").upper()
        stock = lookup(symbol)

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("Myssing symbol")

        # Ensure symbol submitted is valid
        elif not stock:
            return apology("Invalid symbol")

        # Ensure shares was submitted
        elif not request.form.get("shares"):
            return apology("Must provide shares")

        # Ensure shares submitted is a positiva number
        elif not request.form.get("shares").isdigit():
            return apology("Must provide positve shares")

        # Process to buy
        else:

            tabla = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])

            cash = tabla[0]["cash"]

            updated_cash = cash - (shares * stock["price"])

            # Ensure money submitted can be afforded
            if updated_cash < 0:
                return apology("Can't afford")

            else:
                db.execute("UPDATE users SET cash=:updated_cash WHERE id=:id", updated_cash=updated_cash, id=session["user_id"])

                db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (:user_id, :symbol, :shares, :price)", user_id=session["user_id"], symbol=stock["symbol"], shares=shares, price=stock["price"])

                # Redirect user to home page
                flash("Bought!")
                return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    if request.method == "POST":
        db.execute("""
            UPDATE users
            SET cash = cash + :amount
            WHERE id=:user_id
        """, amount = request.form.get("cash"),
        user_id = session["user_id"])

        flash("Added Cash!")
        return redirect("/")
    else:
        return render_template("add_cash.html")




@app.route("/history")
@login_required
def history():

    """Show history of transactions"""

    user_id = session["user_id"]

    transactions = db.execute("SELECT * FROM transactions WHERE user_id = :user_id", user_id=user_id)

    for stock in transactions:
        the = lookup(stock["symbol"])
        stock["name"] = the["name"]
        stock["price"] = usd(stock["price"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    """Get stock quote."""

    if request.method == "POST":

        symbol = request.form.get("symbol").upper()

        # Ensure symbol was submitted
        if not symbol:
            return apology("Missing symbol")

        stock = lookup(request.form.get("symbol"))

        # Ensure stock was a valid symbol
        if not stock:
            return apology("Invalid symbol")

        # If stock was a valid symbol, return what was looking for
        else:
            return render_template("quoted.html", stock={
               'name': stock['name'],
               'symbol': stock['symbol'],
               'price': usd(stock['price'])
            })

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():

    """Register user"""

    if request.method == "POST":

        Username = request.form.get("username")
        PassWord = request.form.get("password")

        # Ensure username was submitted
        if not Username:
            return apology("You must provide an username")

        # Ensure password was submitted
        elif not PassWord:
            return apology("Missing password")

        # Ensure password confimation was submitted
        elif not request.form.get("confirmation"):
            return apology("You must confirm your password")

        # Ensure that password and its confirmation match
        elif PassWord != request.form.get("confirmation"):
            return apology("Password does not match")

        # Ensure udername does not already exist
        elif db.execute("SELECT * FROM users WHERE username = :username",
                        username=Username):
            return apology("Username already exists")

        # Ensure password is hashed for security
        hashed_password = generate_password_hash(PassWord)

        # Save username into db
        table = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=Username, hash=hashed_password)

        session["user_id"] = table

        # Redirect user to home page
        flash("Registered!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    """Sell shares of stock"""

    if request.method == "POST":

        shares = float(request.form.get("shares"))
        symbol = request.form.get("symbol").upper()
        stock = lookup(symbol)

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("Myssing symbol")

        # Ensure symbol submitted is valid
        elif not stock:
            return apology("Invalid symbol")

        # Ensure shares was submitted
        elif not request.form.get("shares"):
            return apology("Must provide shares")

        # Ensure shares submitted is a positiva number
        elif not request.form.get("shares").isdigit():
            return apology("Must provide positve shares")

        # Process to sell
        else:

            rows = db.execute("""
                SELECT symbol, SUM(shares) AS totalShares
                FROM transactions
                WHERE user_id=:user_id
                GROUP BY symbol
                HAVING totalShares > 0;
            """, user_id=session["user_id"])

            for row in rows:
                if row["symbol"] == symbol:
                    if shares > row["totalShares"]:
                        return apology("Invalid number, too many shares")

            tabla = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])

            cash = tabla[0]["cash"]

            updated_cash = cash + (shares * stock["price"])



            db.execute("UPDATE users SET cash=:updated_cash WHERE id=:id", updated_cash=updated_cash, id=session["user_id"])

            db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (:user_id, :symbol, :shares, :price)", user_id=session["user_id"], symbol=stock["symbol"], shares= -1 * shares, price=stock["price"])

            # Redirect user to home page
            flash("Sold!")
            return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        all_symbols = db.execute("""
            SELECT symbol
            FROM transactions
            WHERE user_id=:user_id
            GROUP BY symbol
            HAVING SUM(shares) > 0;
        """, user_id=session["user_id"])
        return render_template("sell.html", symbols = [that_symbol["symbol"] for that_symbol in all_symbols])



def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
