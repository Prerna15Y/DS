import socket

SERVER_ADDRESS=("localhost",8080)
BUFFER_SIZE=1024

class TokenRingClient:
    def __init__(self):
        self.client_socket=socket.socket(socket.AF_INET,socket.SOCK_STREAM)

    
    def connect(self):
        self.client_socket.connect(SERVER_ADDRESS)
        print("connected to server")

    def start(self):
        try:
            while True:
                data=self.client_socket.recv(BUFFER_SIZE).decode()
                if data=="TOKEN":
                    print("tokeb received.accessing resource")
                    print("working on the resource")

                    import time
                    time.sleep(5)

                    print("resource access complete.releasing token")
                    self.client_socket.send("TOKEN".encode())

                if data=="CLOSE":
                    print("closing client")
                    self.stop()
                    break

        except KeyboardInterrupt:
            print("closing client")
            self.client_socket.send("CLOSE".encode())
            self.stop()


    def stop(self):
        self.client_socket.close()


if __name__=="__main__":
    client=TokenRingClient()
    client.connect()
    client.start()
