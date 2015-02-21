<h4>
	<b>Compiling server:<b>
</h4>

<p>
go run server.go accountmanager.go gamestructure.go
</p>

<br>

<h4>
	<b>Compiling client:<b>
</h4>

<p>
g++ -std=c++11 -pthread -o client client.cpp connection.cpp login.cpp json11/json11.cpp
</p>
