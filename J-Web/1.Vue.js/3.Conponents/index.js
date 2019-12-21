Vue.component('todo-item', {
    props: ['todo', 'key', 'abc', 'test_key'],        // 这个todo和v-bind:todo对应 ？！
    template:'<li>{{ todo.id }}-{{ todo.text }}-{{ key }}-{{ test_key }}-{{ abc }}</li>'    // key无法打印！！！为什么？
})

var app = new Vue({
    el: '#app',
    data: {
        groceryList: [
            { id: 0, text: '蔬菜' },
            { id: 1, text: '奶酪' },
            { id: 2, text: '饭饭' }
        ]
    }
})