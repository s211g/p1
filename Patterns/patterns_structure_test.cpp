#include <iostream>

#include <fstream>
#include <filesystem>

#include "pattern_adapter.hpp"
#include "pattern_bridge.hpp"
#include "pattern_composite.hpp"
#include "pattern_decorator.hpp"
#include "pattern_facade.hpp"
#include "pattern_flyweight.hpp"
#include "pattern_proxy.hpp"

namespace patterns_structure_test {

    void test_adapter() {
        std::cout << "\ntest adapter" << std::endl;

        using namespace pattern_adapter;

        std::cout << "\ntest 1" << std::endl;
        // Адаптер класса
        // для адаптации используется множественное неследование
        // наследование по одной ветке, реализация по другой

        class A {
        public:
            void f3() { std::cout << "Target::f3()" << std::endl; }
            void f4() { std::cout << "Target::f4()" << std::endl; }
        };

        class Target : public AdapterInterface1, private A {
        public:
            void f1() override {
                std::cout << "adapted A" << std::endl;
                A::f3();
            };
            void f2() override {
                std::cout << "adapted A" << std::endl;
                A::f3();
            };
        };

        Target t;
        t.f1();
        t.f2();

        std::cout << "\ntest 2" << std::endl;
        // Адаптер объекта
        // для адаптации используется композиция
        // может работать со множеством объектов
        // может использоваться в качестве манипулятора объекта

        using AdaptInterface = AdapterInterface2<A>;
        class Manipulator : public AdaptInterface {
        public:
            Manipulator(A* a) :
                AdaptInterface(a) {}
            void f1() override {
                std::cout << "Adapt+" << std::endl;
                obj->f3();
            }
            void f2() override {
                std::cout << "Adapt+" << std::endl;
                obj->f4();
            }

            void manipulate() {
                std::cout << "manipulate obj:" << std::endl;
                obj->f4();
                obj->f3();
            }
        };

        A a;
        Manipulator man(&a);
        man.f1();
        man.f2();
        man.manipulate();
    }

    using namespace pattern_bridge;

    class FileFlusher : public FlusherInterface {
    public:
        void Open(std::string command) override { file.open(command); }
        void Close() override { file.close(); }
        void Write(std::string text) override { file << text; };

    private:
        std::ofstream file;
    };

    class ConsoleFlusher : public FlusherInterface {
    public:
        void Open(std::string command) override {}
        void Close() override { std::cout << std::flush; }
        void Write(std::string text) override { std::cout << text; };
    };

    FlusherInterface* FlusherFabric(FlusherType type) {
        switch (type) {
            case FlusherType::FILE:
                return new FileFlusher();
            case FlusherType::CONSOLE:
                return new ConsoleFlusher();
            default:
                return nullptr;
        }
    }

    void test_bridge() {
        std::cout << "\ntest bridge" << std::endl;

        // назначение:
        // отделить абстракцию от реализации
        // реализация может быть скрыта от клиента
        // реализацию можно поменять динамически
        // абстракция не обязательно должна иметь одинаковый интерфейс с реализацией

        std::cout << "\ntest 1" << std::endl;
        FlushBrige fb_console(&FlusherFabric, FlusherType::CONSOLE);
        fb_console.Open("");
        fb_console.Write("HI\n");
        fb_console.Close();

        std::cout << "\ntest 2" << std::endl;
        std::string filename = "test_brige.txt";
        std::cout << "write to file: " << std::filesystem::current_path().generic_string() << "/"
                  << filename << std::endl;
        FlushBrige fb_file(&FlusherFabric, FlusherType::FILE);
        fb_file.Open(filename);
        fb_file.Write("HI\n");
        fb_file.Close();
    }

    void test_composite() {
        std::cout << "\ntest composite" << std::endl;
        using namespace pattern_composite;

        // назначение:
        // компонует объекты в древовидные структуры для представления иерархий часть-целое.
        // позволяет клиентам единообразно трактовать индивидуальные и составные объекты
        // позволяет группировать мелкие объекты в более крупные которые в свою очередь могут стать основой для еще более крупных
        // ключем к паттерну я вляется абстрактный класс который представляет одновременно и примитивы и контейнеры

        std::cout << "\ntest 1" << std::endl;

        auto computer = new CompositeEquipment("computer");
        computer->Add(new ItemEquipment("Supply", 10));

        auto mainboard = new CompositeEquipment("Mainboard");
        mainboard->Add(new ItemEquipment("Memory8Gb", 10));
        mainboard->Add(new ItemEquipment("Memory16Gb", 20));
        mainboard->Add(new ItemEquipment("VideoCard", 100));
        computer->Add(mainboard);

        auto pciex_raid = new CompositeEquipment("Raid");
        pciex_raid->Add(new ItemEquipment("Hdd_1TB", 10));
        pciex_raid->Add(new ItemEquipment("Hdd_2TB", 20));
        mainboard->Add(pciex_raid);

        std::cout << "computer description:" << std::endl;
        std::cout << computer->Description() << std::endl;
        std::cout << "computer power consumption: " << computer->Power() << " Watt" << std::endl;

        auto FindByName = [&](std::string name) {
            auto x = computer->GetItemByName(name);
            if (x)
                std::cout << "item type: " << (dynamic_cast<const CompositeEquipment*>(x) ? "Composite" : "Item")
                          << ", description: " << x->Description() << std::endl;
            else
                std::cout << "item " << name << " not found" << std::endl;
        };

        FindByName("Raid");
        FindByName("Raid2");
        FindByName("Memory8Gb");
    }

    void test_decorator() {
        std::cout << "\ntest decorator" << std::endl;
        using namespace pattern_decorator;

        // decorator(wrapper)
        // назначение:
        // динамически добавляет новые обязанности, как альтернатива порождению подклассов

        class Component : public ComponentInterface {
        public:
            Component(std::string text_) :
                text(text_) {}
            std::string GetText() override { return text; }

        private:
            std::string text;
        };

        class ComponentDecoratorPrefix : public ComponentInterface {
        public:
            ComponentDecoratorPrefix(ComponentInterface* obj_) :
                obj(obj_) {}
            std::string GetText() override { return "prefix_" + obj->GetText(); }

        private:
            ComponentInterface* obj;
        };

        class ComponentDecoratorSuffix : public ComponentInterface {
        public:
            ComponentDecoratorSuffix(ComponentInterface* obj_) :
                obj(obj_) {}
            std::string GetText() override { return obj->GetText() + "_suffix"; }

        private:
            ComponentInterface* obj;
        };

        std::cout << "\ntest 1" << std::endl;
        auto component   = new Component("Component");
        auto d_component = new ComponentDecoratorPrefix(new ComponentDecoratorSuffix(component));
        std::cout << "component text: " << d_component->GetText() << std::endl;
    }

    void test_facade() {
        std::cout << "\ntest facade" << std::endl;
        using namespace pattern_facade;

        // назначение:
        // интерфейс более высокого уровня вместо набора интерфейсов некой подсистемы

        std::cout << "\ntest 1" << std::endl;
    }

    void test_flyweight() {
        std::cout << "\ntest flyweight" << std::endl;
        using namespace pattern_flyweight;

        // назначение:
        // применяет совместное использование для эффективной поддержки множества мелких объектов
        // оптимизирует использование большого количества мелких объектов с целью уменьшения затрат ресурсов
        // внутреннее состояние хранится в самом приспособленце и состоит из информации не зависящей от его контекста
        // внешнее состояние зависит от контекста и изменяется вместе с ним, поэтому совместно не используется
        // обекты-клиенты отвечают за передачу внешнего состояния приспособленцу

        // 1 Паттерн разделяет данные объектов на две части — легковесы и контексты.
        // 2 Легковес:
        //             Cодержит состояние(внутреннее), которое повторялось бы во множестве первоначальных объектов.
        //             Всё внутреннее состояние легковес должен получать через параметры конструктора.
        //             Он не должен иметь сеттеров и публичных полей.
        //             Для удобства работы с легковесами и контекстами можно создать фабричный метод
        // 3 Контекст:
        //            Cодержит «внешнюю» часть состояния, уникальную для каждого объекта.
        //            Контекст связан с одним из объектов-легковесов, хранящих оставшееся состояние.
        //            Клиент вычисляет или хранит контекст, то есть внешнее состояние легковесов.
        // Поведение оригинального объекта чаще всего оставляют в Легковесе, передавая значения контекста через параметры методов.
        // Тем не менее, поведение можно поместить и в контекст, используя легковес как объект данных.

        std::cout << "\ntest 1" << std::endl;
        // тараканьи бега
        CockroachFactory factory;
        // 3 типа тараканов(flyweight объекты)
        factory.AddCockroach(1, "A");
        factory.AddCockroach(2, "B");
        factory.AddCockroach(3, "C");

        // 10 тараканов в забеге, а могло быть 100 милионов, у каждого в контексте сохраняется своя позиция
        std::vector<CockroachContext> cockroachs_ctx;
        int count = 10;
        while (count--) {
            std::string name = "Tarakan" + std::to_string(count);
            int type         = count % factory.GetCount();
            cockroachs_ctx.push_back({name, type, 0});
        }

        // забег
        for (int i = 0; i < 20; ++i) {
            std::cout << "it = " << i << " -------------------------------------" << std::endl;
            for (auto& ctx : cockroachs_ctx) {
                auto fw = factory.GetCockroach(ctx.type);
                fw->display(ctx);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    void test_proxy() {
        std::cout << "\ntest proxy" << std::endl;
        using namespace pattern_proxy;

        // proxy(surrogate)
        // назначение
        // заменяет реальный объект
        // контроль доступа к объекту

        // применимость:
        //      Ленивая инициализация (виртуальный прокси). Когда у вас есть тяжёлый объект, грузящий данные из файловой системы или базы данных.
        //      Защита доступа
        //      Локальный запуск сервиса (удалённый прокси). Когда настоящий сервисный объект находится на удалённом сервере.
        //      Логирование запросов (логирующий прокси). Когда требуется хранить историю обращений к сервисному объекту.
        //      Кеширование объектов («умная» ссылка). Когда нужно кешировать результаты запросов клиентов и управлять их жизненным циклом.


        class FileProxy {
        public:
            void open(std::string path_) {
                path = path_;
            }
            void write(std::string text_) {
                text += text_;
            }
            void close() {
                std::ofstream file;
                file.open(path);
                file << text;
                file.close();
            }

        private:
            std::string text;
            std::string path;
        };

        // пример ленивой инициализации
        std::cout << "\ntest 1" << std::endl;
        FileProxy fp;
        std::string filename = "test_proxy.txt";
        fp.open(filename);
        fp.write("1");
        fp.write("2");
        fp.write("3");
        fp.close();
        std::cout << "write to file: " << std::filesystem::current_path().generic_string() << "/"
                  << filename << std::endl;
    }
}
