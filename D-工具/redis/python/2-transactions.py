import asyncio
import aioredis
import redis

def redis_transactions():
    r = redis.Redis(host="localhost", port=6379)
    p = r.pipeline()
    p.watch('redis_conn_set_get', 'redis_connpoll_set_get') # 监听变化
    str1 = p.get('redis_conn_set_get')
    str2 = p.get('redis_connpool_set_get')
    print(str1, str2)
    p.multi()
    newstr1 = str1 + str2
    p.set('str1', newstr1)
    p.execute()
    value = r.get('str1').decode('utf-8')
    print(value)

async def aioredis_transactions():
    redis = await aioredis.create_redis_pool('redis://localhost')

    tr = redis.multi_exec()
    tr.set('key1', 'value1')
    tr.set('key2', 'value2')
    ok1, ok2 = await tr.execute()
    print(ok1, ok2)


if __name__ == "__main__":
    asyncio.run(aioredis_transactions())
    redis_transactions()
