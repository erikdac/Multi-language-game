<font size="16">
	<b>Compiling server:<b>
</font>
</br>
go run server.go accountmanager.go gamestructure.go

<font size="16">
	<b>Compiling client:<b>
</font>
</br>
g++ -std=c++11 -pthread -o client client.cpp connection.cpp login.cpp json11/json11.cpp

