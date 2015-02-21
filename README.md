<b>Compiling server:</b> </br>
go run server.go accountmanager.go gamestructure.go

Compiling client:
g++ -std=c++11 -pthread -o client client.cpp connection.cpp login.cpp json11/json11.cpp

