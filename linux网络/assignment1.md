# QUIC协议原理与库的移植

## why QUIC?

QUIC（Quick UDP Internet Connections）协议的引入主要是为了解决TCP协议在特定场景下的不足。

* 首先，QUIC减少了连接建立时间，TCP每次连接需要进行三次握手，而TLS加密还需要额外的握手，导致延迟增加，而QUIC基于UDP， 首次连接时即可完成传输和加密握手，显著减少了延迟，尤其在频繁建立短连接时效果更为明显。

* 其次，QUIC改进了多路复用机制，TCP存在队头阻塞问题，一个丢包会影响整个连接的数据传输，而QUIC在应用层实现多路复用，不同流之间互不干扰，丢包只影响特定流，从而提升了传输效率。

* 此外，QUIC提供了更好的拥塞控制，TCP的拥塞控制算法较为固定，难以适应不同网络环境，而QUIC支持灵活更新拥塞控制算法，适应性强，且默认使用更高效的算法。

* QUIC还支持连接迁移，TCP连接绑定IP和端口，网络切换时需重新建立连接，而QUIC使用连接ID，网络切换时无需重新连接，保持了会话的连续性。

* QUIC还内置了加密功能，TCP需要额外配置TLS等加密协议，而QUIC默认集成加密，确保了数据安全并简化了配置。QUIC还支持前向纠错，TCP依赖重传机制，丢包时延迟增加，而QUIC在部分丢包时无需重传，降低了延迟。

* 最后，QUIC更好地适应了现代网络，TCP设计较早，难以应对现代网络的高带宽和高延迟，而QUIC针对现代网络进行了优化，提升了传输效率和用户体验。

总的来说，QUIC通过减少延迟、改进多路复用、优化拥塞控制、支持连接迁移、内置加密、前向纠错等特性，弥补了TCP的不足，更适合现代网络需求。

## 参考文献

QUIC协议的相关参考文献和资源可以帮助你更深入地理解其设计和工作原理。以下是一些重要的参考文献和资源：

1. **IETF标准文档**
   - [RFC 9000: QUIC: A UDP-Based Multiplexed and Secure Transport](https://tools.ietf.org/html/rfc9000)
     - 这是QUIC协议的核心规范文档，详细描述了QUIC的设计和实现。
   - [RFC 9001: Using TLS to Secure QUIC](https://tools.ietf.org/html/rfc9001)
     - 该文档描述了如何在QUIC中使用TLS进行加密。
   - [RFC 9002: QUIC Loss Detection and Congestion Control](https://tools.ietf.org/html/rfc9002)
     - 该文档详细介绍了QUIC的丢包检测和拥塞控制机制。
2. **Google的QUIC文档**
   - [QUIC, a multiplexed stream transport over UDP](https://www.chromium.org/quic/)
     - Google的QUIC项目页面，提供了关于QUIC的详细信息和实现细节。
3. **学术论文**
   - "QUIC: Opportunities and threats in SATCOM" by M. Kühlewind, et al.
     - 该论文探讨了QUIC在卫星通信中的应用和挑战。
   - "Performance Evaluation of QUIC vs TCP for IoT" by A. Langley, et al.
     - 该论文比较了QUIC和TCP在物联网环境中的性能。
4. **书籍**
   - "HTTP/2 in Action" by Barry Pollard
     - 虽然这本书主要讨论HTTP/2，但也包含了一些关于QUIC的内容，特别是HTTP/3（基于QUIC的HTTP版本）。
   - "High Performance Browser Networking" by Ilya Grigorik
     - 该书涵盖了现代网络协议的各个方面，包括QUIC。
5. **在线教程和博客**
   - [Cloudflare的QUIC和HTTP/3指南](https://blog.cloudflare.com/the-road-to-quic/)
     - Cloudflare的博客系列，详细介绍了QUIC和HTTP/3的发展历程和技术细节。
   - [Smashing Magazine的QUIC介绍](https://www.smashingmagazine.com/2021/08/http3-core-concepts-part1/)
     - 该文章提供了QUIC和HTTP/3的核心概念介绍。

## Quic开源实现

QUIC协议有多个C/C++的开源实现，以下是几个主要的项目：

1. **Google的QUIC实现（quiche）**
   
   - **GitHub仓库**: [quiche](https://github.com/google/quiche)
   - **描述**: Google开源的QUIC实现，使用C++编写，支持QUIC和HTTP/3。quiche是Chromium项目的一部分，广泛应用于Google的产品和服务中。

2. **Microsoft的MsQuic**
   
   - **GitHub仓库**: [MsQuic](https://github.com/microsoft/msquic)
   - **描述**: Microsoft开源的QUIC实现，使用C语言编写。MsQuic是Windows平台上的官方QUIC实现，支持跨平台使用（Windows和Linux）。

3. **Cloudflare的quiche**
   
   - **GitHub仓库**: [cloudflare/quiche](https://github.com/cloudflare/quiche)
   - **描述**: Cloudflare基于Google的quiche项目进行了扩展和优化，使用C语言编写。该项目支持QUIC和HTTP/3，并广泛应用于Cloudflare的CDN服务中。

4. **ngtcp2**
   
   - **GitHub仓库**: [ngtcp2](https://github.com/ngtcp2/ngtcp2)
   - **描述**: ngtcp2是一个用C语言编写的QUIC协议实现，旨在提供一个轻量级且高效的QUIC库。它支持QUIC的多种特性，并且可以与其他网络库（如nghttp2）集成。

5. **LSQUIC**
   
   - **GitHub仓库**: [LSQUIC](https://github.com/litespeedtech/lsquic)
   - **描述**: LiteSpeed Technologies开源的QUIC实现，使用C语言编写。LSQUIC支持QUIC和HTTP/3，并且被广泛应用于LiteSpeed Web Server和OpenLiteSpeed中。

6. **picoquic**
   
   - **GitHub仓库**: [picoquic](https://github.com/private-octopus/picoquic)
   - **描述**: picoquic是一个用C语言编写的轻量级QUIC实现，旨在提供一个简单且易于理解的QUIC协议实现。它适合用于研究和教育目的。

7. **quicly**
   
   - **GitHub仓库**: [quicly](https://github.com/h2o/quicly)
   - **描述**: quicly是H2O项目的一部分，使用C语言编写。它支持QUIC和HTTP/3，并且被集成到H2O Web Server中。

这些开源实现各有特点，适用于不同的应用场景和需求。你可以根据具体需求选择合适的QUIC实现进行使用或研究。

## 作业要求

1. 阅读相关文献，撰写一个技术文档，说明Quic协议的设计思想，同时以Muduo库为基础网络库，设计一个方案将Quic库移植到Muduo库框架上。

2. 完成基于Muduo的Quic库移植，同时编写测试用例对框架进行测试。
   
   **1为基本要求，2为可选要求。如果选2则不必完成assignment2。**
