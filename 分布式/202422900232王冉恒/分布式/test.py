import http
import json
import socketserver
import sys
import threading
from http.server import BaseHTTPRequestHandler
import grpc
import cache_pb2 as pb2
import cache_pb2_grpc as pb2_grpc
from concurrent import futures

base_ip = "127.0.0.1:"
client_Address = ""
server_Address = ""

class GrpcServer(pb2_grpc.CacheServicer):
    def __init__(self):
        super().__init__()
        self.store = {}
    def SetCache(self, request, context):
        self.store[request.key] = request.value
        print(self.store[request.key])
        return pb2.SetReply()
    def GetCache(self, request,context):
        print(self.store.get(request.key, 'N/A'))
        return pb2.GetReply(key=request.key, value=self.store.get(request.key, 'N/A'))
    def DeleteCache(self, request,context):
         if request.key in self.store.keys():
             self.store.pop(request.key)
             return pb2.DeleteReply(num = 1)
         else:
             return pb2.DeleteReply(num = 0)

def StartRpcServer():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=3))
    pb2_grpc.add_CacheServicer_to_server(GrpcServer(), server)
    server.add_insecure_port(server_Address)
    print("服务端启动，ip为 ",server_Address)
    server.start()
    server.wait_for_termination()

class HttpRequestHandler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        key = self.path.strip("/")
        flag = 1
        value = ""
        for i in range(int(3)):
            tmp_address = base_ip+str(int(i) + 8081)
            channel = grpc.insecure_channel(tmp_address)
            server = pb2_grpc.CacheStub(channel)
            value = server.GetCache(pb2.GetRequest(key=key)).value
            if value != 'N/A':
                flag = 0
                break

        if flag == 0:
            self.send_response(200)
            self.send_header('Content-type', 'application/json;charset = utf-8')
            self.end_headers()

            data = {key: value}
            json_data = json.dumps(data)
            self.wfile.write(json_data.encode(encoding='gbk'))
        else:
            self.send_response(404)
            self.send_header('Content-type', 'application/json;charset = utf-8')
            self.end_headers()
            self.wfile.write("\n".encode(encoding='utf-8'))

    def do_POST(self):
        body = self.rfile.read(int(self.headers['Content-Length']))
        jsonstr = json.loads(body.decode(encoding='utf-8'))
        key1 = ''
        for k in jsonstr.keys():
            key1 = k

        value = str(jsonstr.get(key1))
        channel = grpc.insecure_channel(server_Address)
        server = pb2_grpc.CacheStub(channel)

        server.SetCache(pb2.SetRequest(key=key1, value=value))
        self.send_response(200)
        self.send_header('Content-type', 'application/json;charset = utf-8')
        self.end_headers()
        self.wfile.write(json.dumps(jsonstr).encode(encoding='gbk'))

    def do_DELETE(self):
        print(self.path)
        key = self.path.strip("/")
        self.send_response(200)
        self.send_header('Content-type', 'application/html;charset = utf-8')
        self.end_headers()

        flag = 1
        for i in range(int(3)):
            tmp_address = base_ip+str(int(i) + 8081)
            channel = grpc.insecure_channel(tmp_address)
            server = pb2_grpc.CacheStub(channel)
            value = server.GetCache(pb2.GetRequest(key=key))
            if value.value != 'N/A':
                server.DeleteCache(pb2.DeleteRequest(key=key))
                flag = 0
        if flag == 0:
            self.wfile.write("1".encode('utf-8'))
        else:
            self.wfile.write("0".encode('utf-8'))

def StartClientServer():
    handler = HttpRequestHandler
    port = (int)(sys.argv[1]) + 9526
    httpd = socketserver.TCPServer((base_ip.split(':')[0],port),handler)
    print("客户端启动，ip为:",base_ip+str(port))
    httpd.serve_forever()

if __name__ == "__main__":
    if len(sys.argv) != 2 | (int(sys.argv[1])>3):
        print("应该输入大小为1到3的参数")
    else:
        client_Address = base_ip+str(int(sys.argv[1]) + 9526)
        server_Address = base_ip+str(int(sys.argv[1]) + 8080)

        http_thread = threading.Thread(target=StartClientServer)
        rpc_thread = threading.Thread(target=StartRpcServer)

        http_thread.start()
        rpc_thread.start()
        http_thread.join()
        rpc_thread.join()