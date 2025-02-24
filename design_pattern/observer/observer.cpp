#include <iostream>
#include <vector>
#include <string>

// 观察者接口
class Observer {
public:
    virtual void update(float temperature, float humidity) = 0;
    virtual ~Observer() {}
};

// 主题接口
class Subject {
public:
    virtual void addObserver(Observer* observer) = 0;
    virtual void removeObserver(Observer* observer) = 0;
    virtual void notifyObservers() = 0;
    virtual ~Subject() {}
};

// 具体主题（WeatherStation）
class WeatherStation : public Subject {
private:
    std::vector<Observer*> observers;  // 存储所有的观察者
    float temperature;  // 温度
    float humidity;     // 湿度

public:
    void setWeatherData(float temp, float hum) {
        temperature = temp;
        humidity = hum;
        notifyObservers();  // 数据变化，通知观察者
    }

    void addObserver(Observer* observer) override {
        observers.push_back(observer);
    }

    void removeObserver(Observer* observer) override {
        for(int i=0;i<observers.size();i++){
            if(observers[i]==observer){
                observers.erase(observers.begin()+i);
            }
        }
    }

    void notifyObservers() override {
        for (auto* observer : observers) {
            observer->update(temperature, humidity);
        }
    }
};


// 具体观察者（TemperatureDisplay）
class TemperatureDisplay : public Observer {
public:
    void update(float temperature, float humidity) override {
        std::cout << "Temperature Display: Temperature = " << temperature << "°C\n";
    }
};

// 具体观察者（HumidityDisplay）
class HumidityDisplay : public Observer {
public:
    void update(float temperature, float humidity) override {
        std::cout << "Humidity Display: Humidity = " << humidity << "%\n";
    }
};

int main() {
    // 创建具体主题（气象站）
    WeatherStation weatherStation;

    // 创建观察者
    TemperatureDisplay tempDisplay;
    HumidityDisplay humDisplay;

    // 注册观察者
    weatherStation.addObserver(&tempDisplay);
    weatherStation.addObserver(&humDisplay);

    // 设置新的气象数据（温度和湿度），这将自动通知观察者
    weatherStation.setWeatherData(25.5f, 60.0f);
    weatherStation.setWeatherData(22.3f, 55.0f);

    // 注销一个观察者
    weatherStation.removeObserver(&humDisplay);

    // 设置新的气象数据，这次只有温度显示会被更新
    weatherStation.setWeatherData(20.0f, 50.0f);

    return 0;
}
