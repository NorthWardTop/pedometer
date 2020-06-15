
#!/bin/sh

ps -efww | grep -w 'pedometer' | grep -v grep | cut -c 9-15 | xargs kill -9
./pedometer





