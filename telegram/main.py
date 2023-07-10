import json
import paho.mqtt.client as mqtt
import logging.handlers

import config
from chatbot import ChatBot
from tempmanager import temperatureManager
from lightmanager import lightManager

logging.basicConfig(
    level=logging.DEBUG, format="%(asctime)s - %(name)s - %(levelname)s - %(message)s"
)

logger = logging.getLogger()
logger.setLevel(logging.DEBUG)

# fh = logging.FileHandler('fl30door.log')
fh = logging.handlers.TimedRotatingFileHandler(
    filename="mqttlogic.log", when="midnight", backupCount=10
)
fh.setFormatter(
    logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
)

fh.setLevel(logging.DEBUG)

logger.addHandler(fh)


# The callback for when the client receives a CONNACK response from the server.
def on_mqtt_connect(client, userdata, flags, rc):
    logger.info("Connected with result code " + str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("demo/#")
    client.subscribe("remoanton/rooms/+/mode")
    client.subscribe("remoanton/rooms/+/color")
    client.subscribe("remoanton/rooms/#")


# The callback for when a PUBLISH message is received from the server.
def on_mqtt_message(client, userdata, msg):
    logger.debug(msg.topic + " " + str(msg.payload))
    logger.debug(f"{msg.topic} {msg.payload}")
    if msg.topic.startswith("remoanton/rooms/"):
        logger.debug("Received rooms")
        room = msg.topic.split("/")[2]
        attribute = msg.topic.split("/")[-1]
        if attribute == "mode":
            lightManager.update_room_mode(room, msg.payload.decode())
        elif attribute == "color":
            lightManager.update_room_color(room, msg.payload.decode())
        else:
            lightManager.update_room_state(room, msg.payload.decode())
    elif msg.topic == "remoanton/temperature":
        temperature = float(msg.payload)
        temperatureManager.handle_temperature(temperature)
    else:
        logger.debug(f"Unhandled topic {msg.topic}")


def main() -> None:
    """Run MQTT Client"""
    client = mqtt.Client()
    client.on_connect = on_mqtt_connect
    client.on_message = on_mqtt_message

    logger.info(f"Connecting to mqtt server {config.mqtt_host}:{config.mqtt_port}")
    client.connect(config.mqtt_host, config.mqtt_port, 60)

    # Blocking call that processes network traffic, dispatches callbacks and
    # handles reconnecting.
    # Other loop*() functions are available that give a threaded interface and a
    # manual interface.
    client.loop_start()

    logger.info("Starting Telegram bot")

    """Run the bot."""
    # Create the Updater and pass it your bot's token.
    chatbot = ChatBot(config.telegram_token)

    temperatureManager.temperature_changed_significantly_callback = (
        chatbot.send_temperature
    )

    lightManager.light_changed_callback = chatbot.send_light_change

    chatbot.updater.idle()
    logger.info("stopping mqtt client")
    client.loop_stop()


if __name__ == "__main__":
    main()
