apt-get install libgsl0-dev libconfuse-dev libbluetooth-dev libiw-dev libacpi-dev gnuplot-x11 gnuplot

cp -r ./triplos $HOME
cp -r ./triplos/triplos /etc
cd ./Debug
make clean
make
cd ..
cp ./Debug/triplos /bin


