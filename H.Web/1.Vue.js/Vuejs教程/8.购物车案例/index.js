const app = new Vue({
    el: '#app',
    data: {
        message: 'Hello Vue.js!',
        books: [
            {
                id: 1,
                name: 'aaa',
                date: '2021-1',
                price: 11.11,
                count: 1,
            },
            {
                id: 2,
                name: 'bbb',
                date: '2021-2',
                price: 12.00,
                count: 1,
            },
            {
                id: 3,
                name: 'ccc',
                date: '2021-3',
                price: 13.10,
                count: 1,
            },
            {
                id: 4,
                name: 'ddd',
                date: '2021-4',
                price: 14.05,
                count: 1,
            },
        ]
    },
    methods: {
        inc(index) {
            this.books[index].count++;
        },
        dec(index) {
            this.books[index].count--;
        },
        remove(index) {
            this.books.splice(index, 1);
        }
    },
    filters:{
        showPrice(price) {
            return '￥' + price.toFixed(2);
        }
    },
    computed: {
        totalPrice() {
            let result = 0;
            for (let book of this.books) {
                result += book.price * book.count;
            }

            return result;
        }
    }
})
