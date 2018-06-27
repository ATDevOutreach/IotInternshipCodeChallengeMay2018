#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mqtt/client.h"

#include "mraa/common.hpp"
#include "mraa/gpio.hpp"
#include "mraa/aio.hpp"

#define EXIT_FAILURE 1
#define LED_PIN 23
#define LDR_PIN 24

const std::string MQTT_SERVER("tcp://sungura1-angani-ke-host.africastalking.com:10883");
const std::string DEVICE_ID("CoolestDeviceID");
const std::string SENSOR_TOPIC("thor/sensors/ldr");
const std::string COMMAND_TOPIC("thor/command/led");
const std::string USERNAME("thor");
const std::string PASSWORD("avengers");
std::string PAYLOAD("");
const int QOS  = 1;

volatile sig_atomic_t flag = 1;

class callback : public virtual mqtt::callback
{
    mqtt::client& client_;
    void connected(const std::string& cause) override 
    {
        std::cout << "\nConnected:" << cause << std::endl;
        client_.subscribe(COMMAND_TOPIC, QOS);
        std::cout << std::endl;
    }
    void connection_lost(const std::string& cause) override 
    {
        std::cout << "\nConnection Lost";
        
        if (!cause.empty()) 
        {
            std::cout << ":" << cause << std::endl;
        }
        std::cout << std::endl;
    }
    void message_arrived(mqtt::const_message_ptr msg) override
    {
        std::cout << msg->get_topic() << ":" << msg->get_payload_str() << std::endl;
    }

    void delivery_complete(mqtt::delivery_token_ptr token) override {}

    public:
     callback(mqtt::client& cli) : client_(cli) {}
};

void sig_handler(int signum)
{
    
    if (signum == SIGINT)
    {
        std::cout << "Exiting..." << std::endl;
        flag = 0;

    }
    
}


int main(void)
{
    mqtt::connect_options conOpts;
    conOpts.set_keep_alive_interval(20);
    conOpts.set_clean_session(false);
    conOpts.set_automatic_reconnect(true);
    conOpts.set_user_name(USERNAME);
    conOpts.set_password(PASSWORD);

    mqtt::client cli(MQTT_SERVER, CLIENT_ID);

    callback cb(cli);
    cli.set_callback(cb);

    try
    {
        std::cout << "Connecting to Server..." << std::flush;
        cli.connect(conOpts);
        cli.subscribe(COMMAND_TOPIC);
        std::cout << "Connection Success" << std::endl;
    }
    catch(const mqtt::exception& e)
    {
        std::cerr << "\nError Connecting to the Server" << '\n';
        return EXIT_FAILURE;
    }
    mraa::Result status;
    signal(SIGINT, sig_handler);
    mraa::Gpio gpio_1(LED_PIN);
    mraa::Aio aio_1(LDR_PIN);
    status = gpio_1.dir(mraa::DIR_OUT);
    float val = 0.00;
    if (status != mraa::SUCCESS) 
    {
        printError(status);
        return EXIT_FAILURE;
    }
    while (flag) {
  
        auto commands = cli.consume_message();

        
        if (commands->get_topic() == COMMAND_TOPIC)
        {
            
            if (commands ->to_string() == "ON") {
                 status = gpio_1.write(1);
                if (status != mraa::SUCCESS) 
                {
                        printError(status);
                        return EXIT_FAILURE;
                }
                sleep(1);
                break;
            }
            else if(commands->to_string() == "OFF") {
                 status = gpio_1.write(0);
                if (status != mraa::SUCCESS) 
                {
                        printError(status);
                        return EXIT_FAILURE;
                }
                sleep(1);
                break;
            }
            else {
               break;
            }
        }
        
        try
        {         
        val = aio_1.readFloat();
        PAYLOAD = std::to_string(val);
        auto payload = mqtt::make_message(SENSOR_TOPIC, PAYLOAD);
        payload->set_qos(QOS);
        cli.publish(payload);
        std::cout << "OK" << std::endl;
        sleep(1);
        }
        catch(const mqtt::exception& e)
        {
            std::cerr << e.what() << '\n';
            return EXIT_FAILURE;
        }
    }
    cli.disconnect();
    return EXIT_SUCCESS;
}


