https://randomnerdtutorials.com/send-notifications-esp32/

telegram bot setup:
https://randomnerdtutorials.com/telegram-esp32-motion-detection-arduino/
url for sending messages:
https://api.telegram.org/bot7101736617:AAG_SgYUf_kvl_B01ExOz6pWRBOQdaca4dw/sendMessage?chat_id=1807248297&text=Hello%20World_alert_notification
{
    "ok": true,
    "result": {
        "message_id": 7,
        "from": {
            "id": 7101736617,
            "is_bot": true,
            "first_name": "anigno_alerts",
            "username": "anigno_bot"
        },
        "chat": {
            "id": 1807248297,
            "first_name": "roni",
            "username": "anigno",
            "type": "private"
        },
        "date": 1715077488,
        "text": "Hello World!_alert_notification"
    }
}
https://api.telegram.org/bot7101736617:AAG_SgYUf_kvl_B01ExOz6pWRBOQdaca4dw/getMe
{
    "ok": true,
    "result": {
        "id": 7101736617,
        "is_bot": true,
        "first_name": "anigno_alerts",
        "username": "anigno_bot",
        "can_join_groups": true,
        "can_read_all_group_messages": false,
        "supports_inline_queries": false,
        "can_connect_to_business": false
    }
}