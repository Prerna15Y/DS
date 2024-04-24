import socket
import threading

TOKEN="TOKEN"
PORT=8080
BUFFER_SIZE=1024

class TokenRingServer:
    def __init__(self):
        self.server_socket=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.clients=[]
        self.client_threads=[]
        self.running=False
    
    def start(self):
        self.server_socket.bind(("localhost",PORT))
        self.server_socket.listen()
        self.running=True
        print("Server Started. Listening for connections")

        try:
            while self.running:
                client_socket,client_address=self.server_socket.accept()
                print(f"new client connected: {client_address}")
                self.clients.append(client_socket)

                if len(self.clients)==1:
                    client_socket.send(TOKEN.encode())

                thread=threading.Thread(
                    target=self.handle_client,args=(client_socket,)
                )
                thread.start()
                self.client_threads.append(thread)

        except KeyboardInterrupt:
            self.stop()

    
    def handle_client(self,client_socket):
        while self.running:
            data=client_socket.recv(BUFFER_SIZE).decode()
            next_client=self.clients[(self.clients.index(client_socket)+1)%len(self.clients)]

            if data=="CLOSE":
                print(f"Client disconnected: {client_socket.getpeername()}")
                self.clients.remove(client_socket)
                client_socket.close()
                data=TOKEN
                break

            if data=="TOKEN":
                print("Received token")
                if len(self.clients)>=1:
                    if self.running:
                        print("sending token to next client")
                        next_client.send(TOKEN.encode())

                    else:
                        print("server stopped.Not sending token to next client.")
                        break


    def stop(self):
        self.running=False
        print("closing server...")

        for client in self.clients:
            print(f"sending close signal to {client.getpeername()}")
            client.send("CLOSE".encode())
            client.close()

        for thread in self.client_threads:
            thread.join()

        self.server_socket.close()

if __name__=="__main__":
    server=TokenRingServer()
    server.start()
            