# IoT Telegram Intercom

A project designed to monitor changes in the signal of an intercom through an analog input and send notifications via Telegram.

## How it works

The device will receive the signal from the speaker through the analog port; the program will detect voltage changes in the port and triggers a call to the Telegram API, including the BOT Token and Chat ID.

You may need to divide the voltage, as the analog port supports no more than 3.3v; please verify the speaker's voltage before connecting it to the device. [See how to do it](https://ohmslawcalculator.com/voltage-divider-calculator).



## Telegram API

1. [Create your bot](https://core.telegram.org/bots/tutorial#obtain-your-bot-token) and save the token
2. Find your [Chat Id](https://www.alphr.com/find-chat-id-telegram) and save
