# a1Bmc
Benchmark C

Steps to compile and execute:
```sh
sudo apt-get update

sudo apt-get install libssl-dev
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
make
sudo make install
cd ..

git clone https://github.com/danielvilas/bm_c

sudo apt-get install libfann2
sudo apt-get install libfann-dev
sudo apt-get install libfftw3 libfftw3-devel
sudo apt-get install librdkafka-dev librdkafka1
sudo apt-get install libzookeeper-mt2 libzookeeper-mt-dev
sudo apt-get install libjansson-dev
sudo apt-get install libcurl4-openssl-dev

mkdir build
cd build

cmake ../bm_c
make
cd ../bm_c

../build/bmc_full -d 0Initial -p MQTT

```