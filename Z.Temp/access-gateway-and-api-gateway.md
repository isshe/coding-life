# 接入网关（Access Gateway）和 API 网关（API Gateway）的区别

TODO: 待整理。

接入网关（Access Gateway）和 API 网关（API Gateway）是服务网络中用于不同目的的组件。它们的关键区别在于它们的职责、功能和用途。

**接入网关（Access Gateway）:**

1. **安全性和认证**: 接入网关通常负责保护网络，它们可能会处理 VPN 连接、远程用户认证、设备认证和其他与访问控制相关的功能。
2. **协议转换**: 在某些情况下，它们可以提供不同网络协议之间的转换服务。
3. **网络接入**: 它们主要用于控制访问网络资源的权限，可能包括对接入网络的设备进行管理和监控。
4. **负载均衡**: 有时它们也可能负责在多个网络入口之间进行负载均衡。

接入网关侧重于网络层面的接入和安全性，它们是用户和服务之间的第一道门户。

**API 网关（API Gateway）:**

1. **API 管理**: API 网关是微服务架构中的一个组件，管理着服务间的交互，特别是对外提供的 API 接口。
2. **路由**: 它们负责将外部请求路由到正确的服务端点。
3. **协议转换**: 可以提供 REST, SOAP 等不同的 API 协议之间的转换。
4. **限流和配额**: API 网关可以实施限流策略，限制客户端在一定时间内可以发起的请求数量，同时可以处理 API 配额和使用策略。
5. **安全性**: 虽然也处理认证和授权，但 API 网关更关注于 API 层面的安全性，如 OAuth, API keys, JWT 等。
6. **监控和日志**: 它们可以提供 API 调用的监控、日志记录和分析功能。
7. **缓存**: 有些 API 网关还提供对响应的缓存功能，以减少后端服务的负载。

API 网关侧重于应用程序层面的 API 管理和交互，它们提供了一系列用于简化和控制 API 使用的功能。

总的来说，接入网关更偏向于网络和安全层面，而 API 网关偏向于应用层面，管理和优化 API 的使用和交互。在实际使用中，两者的功能可能会有一些重叠，但它们的主要关注点和用途是不同的。
