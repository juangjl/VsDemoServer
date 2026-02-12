touch leave
sleep 2
export app=$(basename "$PWD")
pkill -f $app.o
rm leave
