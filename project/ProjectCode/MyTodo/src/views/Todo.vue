<template>
  <div class="home">
    <v-text-field
      v-model="newTaskTitle"
      @click:append="addTask"
      @keyup.enter="addTask"
      class="pa-5 pt-6 pb-2"
      outlined
      label="Add Task"
      append-icon="mdi-plus"
      hide-details
      clearable
    ></v-text-field>
    <v-list
      class="pt-0"
      flat
    >
      <div v-for="task in tasks" :key="task.id">
        <v-list-item
          @click="doneTask(task.id)"
          :class="{ 'blue lighten-5' : task.done }"
        >
          <template v-slot:default>
            <v-list-item-action>
              <v-checkbox
                :input-value="task.done"
                color="primary"
              ></v-checkbox>
            </v-list-item-action>

            <v-list-item-content>
              <v-list-item-title
                :class="{ 'text-decoration-line-through' : task.done }"
              >
                {{ task.title }}
              </v-list-item-title>
            </v-list-item-content>

            <v-list-item-action>
              <v-btn icon
                @click.stop="deleteTask(task.id)"
              >
                <v-icon color="blue lighten-2">mdi-delete</v-icon>
              </v-btn>
            </v-list-item-action>
          </template>
        </v-list-item>
        <v-divider></v-divider>
      </div>
      
      <v-container
        class="px-0"
        fluid
      >
          <v-subtitle class="pa-6">{{ remaining }} items left</v-subtitle>
        <v-list-item>
          <v-btn class="button alert small ma-6"
            @click="clearAll"
          >
            Clear All
          </v-btn>
        </v-list-item>
      </v-container>
    </v-list>
  </div>
</template>

<script>
export default {
  name: 'Todo',
  data() {
    return {
      newTaskTitle: '',
      tasks: [
        {
          id:1,
          title: "Try to add a new task",
          done: false,
        },
        {
          id:2,
          title: "Enjoy 'My To-do' App",
          done:false,
        },
      ]
    }
  },
  computed: {
    remaining() {
      return this.tasks.filter(task => !task.done).length
    },
    anyRemaining() {
      return this.remaining != 0
    }
  },
  methods: {
    addTask() {
      let newTask = {
        id: Date.now(),
        title: this.newTaskTitle,
        done: false,
      }
      this.tasks.push(newTask)
      this.newTaskTitle = ""
    },
    doneTask(id) {
      let task = this.tasks.filter(task => task.id == id)[0]
      task.done = !task.done
    },
    deleteTask(id) {
      this.tasks = this.tasks.filter(task => task.id != id)
    },
    removeTask(index) {
      this.tasks.splice(index, 1);
    },
    clearAll() {
      this.tasks = [];
    }
  }
}
</script>
