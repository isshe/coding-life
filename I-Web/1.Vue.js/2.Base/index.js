new Vue({
    el: '#app',
    data: {
        seen:true
    }
})

new Vue({
    el: '#app2',
    data: {
        seen:false
    }
})

new Vue({
    el: '#app3',
    data: {
        todos: [
            { text: '学习 JavaScript' },
            { text: '学习 Vue' },
            { text: '整个牛项目' }
        ]
    }
})

new Vue({
    el: '#app4',
    data: {
        message: '页面加载于 ' + new Date().toLocaleString()
    }
})

new Vue({
    el: '#app5',
    data: {
        message: 'Hello Vue.js!'
    },
    methods: {
        reverseMessage: function () {
            this.message = this.message.split('').reverse().join('')
        }
    }
})

new Vue(
    {
        el: '#app6',
        data: {
            message:"Hello isshe"
        },
    }
)
