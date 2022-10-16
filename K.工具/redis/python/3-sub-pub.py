
import asyncio
import aioredis

port = 7001

async def reader(channel):
    async for message in channel.iter():
        print("Got message:", message)

async def reader2(channel):
    async for ch, message in channel.iter():
        print("Got message in channel:", ch, ":", message)


async def aioredis_pub_sub():
    redis = await aioredis.create_redis_pool('redis://localhost:'+str(port))

    ch1, ch2 = await redis.subscribe('channel:1', 'channel:2')
    assert isinstance(ch1, aioredis.Channel)
    assert isinstance(ch2, aioredis.Channel)

    asyncio.get_running_loop().create_task(reader(ch1))
    asyncio.get_running_loop().create_task(reader(ch2))

    await redis.publish('channel:1', 'Hello')
    await redis.publish('channel:2', 'World')

    redis.close()
    await redis.wait_closed()

#
async def aioredis_pub_sub_pattern():
    r = await aioredis.create_redis_pool('redis://localhost:'+str(port))
    ch, = await r.psubscribe('channel:*')
    assert isinstance(ch, aioredis.Channel)

    # for ch in chs:
    asyncio.get_running_loop().create_task(reader2(ch))

    await r.publish('channel:1', 'Hello')
    await r.publish('channel:2', 'World')
    r.close()
    await r.wait_closed()

async def reader3(ch):
    while (await ch.wait_message()):
        msg = await ch.get_json()
        print("Got Message:", msg)


async def aioredis_pub_sub2():
    pub = await aioredis.create_redis(
        'redis://localhost')
    sub = await aioredis.create_redis(
        'redis://localhost')
    res = await sub.subscribe('chan:1')
    ch1 = res[0]

    tsk = asyncio.ensure_future(reader3(ch1))

    res = await pub.publish_json('chan:1', ["Hello", "world"])
    assert res == 1

    await sub.unsubscribe('chan:1')
    await tsk
    sub.close()
    pub.close()

if __name__ == "__main__":
    asyncio.run(aioredis_pub_sub())
    asyncio.run(aioredis_pub_sub2())
    asyncio.run(aioredis_pub_sub_pattern())
