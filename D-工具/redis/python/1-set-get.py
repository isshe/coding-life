import asyncio
import aioredis
import redis

async def aioredis_set_get():
    r = await aioredis.create_redis_pool('redis://localhost:6379') # password="")
    await r.set('my-key', 'value')
    value = await r.get('my-key', encoding='utf-8')
    print(value)

    r.close()
    await r.wait_closed()

def redis_conn_set_get():
    r = redis.Redis(host="localhost", port=6379)
    r.set('redis_conn_set_get', 'redis_conn_set_get')
    value = r.get('redis_conn_set_get').decode('utf-8')
    print(value)

def redis_conn_pool_set_get():
    r = redis.Redis(host="localhost", port=6379)
    r.set('redis_connpool_set_get', 'redis_connpool_set_get')
    value = r.get('redis_connpool_set_get').decode('utf-8')
    print(value)


if __name__ == "__main__":
    asyncio.run(aioredis_set_get())
    redis_conn_set_get()
    redis_conn_pool_set_get()
