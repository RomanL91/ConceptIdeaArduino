#include <iostream>
#include <unistd.h>
#include <stdlib.h>



class Controller {
    public:
        int mod;

        Controller(int p_mod = 0){
            mod = p_mod;
        }

        void print(){
            std::cout << "Mod: " << mod << std::endl;
        }

        void change_mod(int new_mode){
            mod = new_mode;
        }

        bool check_online(){
            if (mod == 1){
                std::cout << "Online TRUE" << std::endl;
                return true;
            } else {
                std::cout << "Online FASLE" << std::endl;
                return false;
            }
        }

        void send_signal_pin(int pin, int output = 1){
            std::cout << "PIN: " << pin << " signal: " << output << std::endl;
        }
};


class Reader: public Controller {
    public:
        // OneWire
        int x;
        int direct; // 1-IN 0-OUT

        Reader(int p_direct = 1){
            direct = p_direct;
        }

        unsigned int random(){
            x = 200 + rand() % 101;
            std::cout << "RANDOM: " << x << " READER: " << direct << std::endl;
            if (x > 250){
                return 0;
            }
            return x;
        }
};


class ServContr: public Controller {
    public:
        int port;
        int ip[4]{};


        ServContr(int p_port, int(&p_ip)[4]){
            port = p_port;
            std::copy(p_ip, p_ip + 4, ip);
        }


        void request(std::string path = "/"){
            // std::cout << "Mod: " << mod << std::endl;
            // std::cout << "adr_ip: " << ip[0] << std::endl;
            // std::cout << "port: " << port << std::endl;
            std::cout << "path: " << path << std::endl;
        }

        void ping(){
            request("/ping");
        }
};

class DataBaseContr: public Controller {
    public:
    int know_card[4] {212, 222, 232, 242}; //типа известные карты которые хранятся в БД контроллера

    void show_DB(){
        std::cout << "card: " << know_card[0] << std::endl;
        std::cout << "card: " << know_card[1] << std::endl;
        std::cout << "card: " << know_card[2] << std::endl;
        std::cout << "card: " << know_card[3] << std::endl;
    }
};


int main() {
    int count = 0;
    int card;
    int adr[4] {192, 168, 0, 129};
    // int count_while = 0;
    Controller controller(0);
    ServContr servcontr(8000, adr);

    Reader readerIN(0); //in
    // Reader readerOUT(0); //out

    DataBaseContr data;

    while (1)
    {
        std::cout << "-----------------START-------------------" << std::endl;
        card = readerIN.random();
        if (controller.check_online()){
            if (card){
                std::cout << "ONLINE: " << controller.check_online() << std::endl;
                servcontr.request("/CHEACK_ACCESS");
            } else {
                std::cout << "NO CARD" << std::endl;
            }
        } else {
            if (card){
                std::cout << "OFFLINE: " << controller.check_online() << std::endl;
                data.show_DB();
                servcontr.send_signal_pin(1);
                servcontr.request();
            } else {
                std::cout << "NO CARD" << std::endl;
            }
        }
        count ++;
        std::cout << "--count--" << count << std::endl;
        if (count > 10) {
            servcontr.ping();
            count = 0;
        }
        std::cout << "------------------END------------------" << std::endl;
        sleep(1);

    }
}
