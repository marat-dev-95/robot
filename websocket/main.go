package main

import (
	"log"
	"net/http"

	"github.com/gorilla/websocket"
)

var upgrader = websocket.Upgrader{
	ReadBufferSize:  1024,
	WriteBufferSize: 1024,
	CheckOrigin: func(r *http.Request) bool {
		return true // Разрешить все источники
	},
}

var globalByteChannel chan []byte

func main() {
	globalByteChannel = make(chan []byte, 10)

	// Установка обработчиков для разных путей
	http.HandleFunc("/send", sendHandler)
	http.HandleFunc("/receive", receiveHandler)

	// Запуск HTTP-сервера на порту 80
	log.Println("Server started on :80")
	log.Fatal(http.ListenAndServe("192.168.197.99:80", nil))
}

func sendHandler(w http.ResponseWriter, r *http.Request) {
	// Принимаем подключение от клиента
	conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Println("Upgrade error:", err)
		return
	}
	defer conn.Close()

	for {
		// Читаем сообщение от клиента
		_, msg, err := conn.ReadMessage()
		if err != nil {
			log.Println("Read error:", err)
			break
		}

		// Отправляем сообщение на путь /receive
		globalByteChannel <- msg
	}
}

func receiveHandler(w http.ResponseWriter, r *http.Request) {
	// Принимаем подключение от клиента
	conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Println("Upgrade error:", err)
		return
	}
	defer conn.Close()

	for {
		msg := <-globalByteChannel
		log.Printf("Received message from sender: %s\n", msg)

		// Отправляем обратно полученное сообщение обратно отправителю
		err = conn.WriteMessage(websocket.TextMessage, msg)
		if err != nil {
			log.Printf("Write error: %v\n", err)
			break
		}
	}
}
