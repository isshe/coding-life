import asyncio
import aioredis

async def aioredis_scan():
    """Scan command example."""
    redis = await aioredis.create_redis(
        'redis://localhost:6379')

    await redis.mset('key:1', 'value1', 'key:2', 'value2')
    cur = b'0'  # set initial cursor to 0
    while cur:
        cur, keys = await redis.scan(cur, match='*')
        print("Iteration results:", keys)

    redis.close()
    await redis.wait_closed()


if __name__ == '__main__':
    asyncio.run(aioredis_scan())