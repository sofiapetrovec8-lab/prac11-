#include <iostream>
#include <string>
using namespace std;

// Дані планети
struct PlanetData {
    string name;
    string type;
    double diameter;
};

// Вузол списку
struct Node {
    PlanetData data;
    Node* next;
};

// Однонапрямлений список
struct List {
    Node* head;
    Node* tail;
    int size;
};

// Лічильники пам'яті
long long allocations = 0;
long long deallocations = 0;

// Початковий стан списку
void initList(List& list) {
    list.head = nullptr;
    list.tail = nullptr;
    list.size = 0;
}

// Створення нового вузла
Node* createNode(PlanetData data) {
    Node* node = new Node;

    node->data = data;
    node->next = nullptr;

    allocations++;

    return node;
}

// Додавання в кінець списку
void addBack(List& list, PlanetData data) {
    Node* node = createNode(data);

    if (list.head == nullptr) {
        list.head = node;
        list.tail = node;
    }
    else {
        list.tail->next = node;
        list.tail = node;
    }

    list.size++;
}

// Додавання на початок списку
void addFront(List& list, PlanetData data) {
    Node* node = createNode(data);

    if (list.head == nullptr) {
        list.head = node;
        list.tail = node;
    }
    else {
        node->next = list.head;
        list.head = node;
    }

    list.size++;
}

// Перевірка однаковості даних
bool sameData(PlanetData a, PlanetData b) {
    return a.name == b.name &&
        a.type == b.type &&
        a.diameter == b.diameter;
}

// Перевірка, чи список порожній
bool isEmpty(List& list) {
    return list.head == nullptr;
}

// Перевірка наявності циклу
bool hasCycle(List& list) {
    Node* slow = list.head;
    Node* fast = list.head;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            return true;
        }
    }

    return false;
}

// Перевірка правильності списку
bool checkList(List& list) {
    if (hasCycle(list)) {
        cout << "Error: cycle detected!" << endl;
        return false;
    }

    int count = 0;
    Node* current = list.head;
    Node* last = nullptr;

    while (current != nullptr && count <= list.size) {
        last = current;
        current = current->next;
        count++;
    }

    if (count != list.size) {
        cout << "Error: incorrect size!" << endl;
        return false;
    }

    if (last != list.tail) {
        cout << "Error: incorrect tail!" << endl;
        return false;
    }

    if (list.tail != nullptr && list.tail->next != nullptr) {
        cout << "Error: tail next is not nullptr!" << endl;
        return false;
    }

    cout << "Structure check: OK" << endl;
    return true;
}

// Перегляд списку
void displayList(List& list) {
    if (isEmpty(list)) {
        cout << "List is empty." << endl;
        return;
    }

    Node* current = list.head;
    int number = 1;

    while (current != nullptr) {
        cout << number << ". "
            << current->data.name << " | "
            << current->data.type << " | "
            << current->data.diameter << endl;

        current = current->next;
        number++;
    }

    cout << "Viewed nodes: " << number - 1 << endl;
}

// Перегляд адрес вузлів
void showLinks(List& list) {
    Node* current = list.head;

    cout << "ADDRESS DIAGNOSTICS" << endl;

    while (current != nullptr) {
        cout << "Current node: "
            << static_cast<void*>(current) << endl;

        cout << "Next node: "
            << static_cast<void*>(current->next) << endl;

        current = current->next;
    }

    cout << "Last node points to nullptr." << endl;
}

// Очищення списку
void clearList(List& list) {
    Node* current = list.head;

    while (current != nullptr) {
        Node* nextNode = current->next;

        delete current;
        deallocations++;

        current = nextNode;
    }

    list.head = nullptr;
    list.tail = nullptr;
    list.size = 0;
}

// Збереження даних для перевірки
void captureValues(List& list, PlanetData values[], int& count) {
    count = 0;

    Node* current = list.head;

    while (current != nullptr && count < 100) {
        values[count] = current->data;
        count++;
        current = current->next;
    }
}

// Перевірка порядку елементів
bool checkOrder(List& list, PlanetData expected[], int count) {
    Node* current = list.head;

    for (int i = 0; i < count; i++) {
        if (current == nullptr) {
            return false;
        }

        if (!sameData(current->data, expected[i])) {
            return false;
        }

        current = current->next;
    }

    return current == nullptr;
}

// Тест 1: порожній список
bool testEmpty() {
    List list;
    initList(list);

    bool result = isEmpty(list) && checkList(list);

    clearList(list);
    return result;
}

// Тест 2: один вузол
bool testOneNode() {
    List list;
    initList(list);

    PlanetData p = { "Earth", "Rocky", 12742 };

    addBack(list, p);

    bool result = list.size == 1 &&
        list.head == list.tail &&
        sameData(list.head->data, p);

    clearList(list);
    return result;
}

// Тест 3: додавання в кінець
bool testAddBack() {
    List list;
    initList(list);

    PlanetData a = { "Earth", "Rocky", 12742 };
    PlanetData b = { "Mars", "Rocky", 6779 };

    addBack(list, a);
    addBack(list, b);

    PlanetData expected[] = { a, b };

    bool result = checkOrder(list, expected, 2);

    clearList(list);
    return result;
}

// Тест 4: додавання на початок
bool testAddFront() {
    List list;
    initList(list);

    PlanetData a = { "Earth", "Rocky", 12742 };
    PlanetData b = { "Mars", "Rocky", 6779 };

    addFront(list, a);
    addFront(list, b);

    PlanetData expected[] = { b, a };

    bool result = checkOrder(list, expected, 2);

    clearList(list);
    return result;
}

// Тест 5: однакові елементи
bool testDuplicates() {
    List list;
    initList(list);

    PlanetData p = { "Earth", "Rocky", 12742 };

    addBack(list, p);
    addBack(list, p);

    bool result = list.size == 2 &&
        sameData(list.head->data, list.tail->data);

    clearList(list);
    return result;
}

// Тест 6: нульовий діаметр
bool testZero() {
    List list;
    initList(list);

    PlanetData p = { "Test", "Unknown", 0 };

    addBack(list, p);

    bool result = list.head->data.diameter == 0;

    clearList(list);
    return result;
}

// Тест 7: від'ємний діаметр
bool testNegative() {
    List list;
    initList(list);

    PlanetData p = { "Test", "Unknown", -10 };

    addBack(list, p);

    bool result = list.head->data.diameter == -10;

    clearList(list);
    return result;
}

// Тест 8: десять вузлів
bool testTenNodes() {
    List list;
    initList(list);

    for (int i = 0; i < 10; i++) {
        PlanetData p = { "Planet", "Type", (double)i };
        addBack(list, p);
    }

    bool result = list.size == 10 && checkList(list);

    clearList(list);
    return result;
}

// Тест 9: повторний перегляд
bool testRepeatedView() {
    List list;
    initList(list);

    PlanetData p = { "Earth", "Rocky", 12742 };

    addBack(list, p);

    Node* oldHead = list.head;

    displayList(list);
    displayList(list);

    bool result = oldHead == list.head;

    clearList(list);
    return result;
}

// Тест 10: сто вузлів
bool testHundredNodes() {
    List list;
    initList(list);

    for (int i = 0; i < 100; i++) {
        PlanetData p = { "Planet", "Type", (double)i };
        addBack(list, p);
    }

    bool result = list.size == 100;

    clearList(list);

    return result && isEmpty(list);
}

// Тест 11: заповнення після очищення
bool testRefill() {
    List list;
    initList(list);

    PlanetData p = { "Earth", "Rocky", 12742 };

    addBack(list, p);
    clearList(list);
    addBack(list, p);

    bool result = list.size == 1 &&
        sameData(list.head->data, p);

    clearList(list);
    return result;
}

// Тест 12: два цикли створення й очищення
bool testTwoCycles() {
    List list;
    initList(list);

    PlanetData p = { "Mars", "Rocky", 6779 };

    addBack(list, p);
    clearList(list);

    addBack(list, p);
    clearList(list);

    return isEmpty(list);
}

// Тест 13: перевірка адрес
bool testAddresses() {
    List list;
    initList(list);

    PlanetData a = { "Earth", "Rocky", 12742 };
    PlanetData b = { "Mars", "Rocky", 6779 };

    addBack(list, a);
    addBack(list, b);

    bool result = list.tail->next == nullptr;

    clearList(list);
    return result;
}

// Тест 14: перевірка хвоста
bool testTail() {
    List list;
    initList(list);

    PlanetData a = { "Earth", "Rocky", 12742 };
    PlanetData b = { "Mars", "Rocky", 6779 };

    addBack(list, a);
    addBack(list, b);

    bool result = list.tail->data.name == "Mars";

    clearList(list);
    return result;
}

// Тест 15: очищення порожнього списку
bool testClearEmpty() {
    List list;
    initList(list);

    clearList(list);

    return list.head == nullptr &&
        list.tail == nullptr &&
        list.size == 0;
}

// Запуск усіх тестів
void runTests() {
    int passed = 0;
    int total = 15;

    cout << "\nAUTOMATED TESTS" << endl;

    if (testEmpty()) passed++;
    if (testOneNode()) passed++;
    if (testAddBack()) passed++;
    if (testAddFront()) passed++;
    if (testDuplicates()) passed++;
    if (testZero()) passed++;
    if (testNegative()) passed++;
    if (testTenNodes()) passed++;
    if (testRepeatedView()) passed++;
    if (testHundredNodes()) passed++;
    if (testRefill()) passed++;
    if (testTwoCycles()) passed++;
    if (testAddresses()) passed++;
    if (testTail()) passed++;
    if (testClearEmpty()) passed++;

    cout << "Passed: " << passed << "/" << total << endl;
}

int main() {
    List list;
    initList(list);

    int n;

    cout << "Enter number of planets: ";
    cin >> n;

    if (cin.fail() || n < 0 || n > 100) {
        cout << "Incorrect number!" << endl;
        return 1;
    }

    PlanetData input[100];

    for (int i = 0; i < n; i++) {
        cout << "\nPlanet " << i + 1 << endl;

        cout << "Name: ";
        cin >> input[i].name;

        cout << "Type: ";
        cin >> input[i].type;

        cout << "Diameter: ";
        cin >> input[i].diameter;

        if (cin.fail()) {
            cout << "Incorrect input!" << endl;
            clearList(list);
            return 1;
        }
    }

    // Створення списку додаванням у кінець
    cout << "\nCREATION: ADD TO END" << endl;

    for (int i = 0; i < n; i++) {
        addBack(list, input[i]);
    }

    displayList(list);
    checkList(list);
    showLinks(list);

    cout << "\nCLEAR LIST" << endl;

    clearList(list);

    cout << "After clear: ";
    checkList(list);

    // Створення списку додаванням на початок
    cout << "\nCREATION: ADD TO BEGINNING" << endl;

    for (int i = 0; i < n; i++) {
        addFront(list, input[i]);
    }

    displayList(list);
    checkList(list);
    showLinks(list);

    cout << "\nCLEAR LIST" << endl;

    clearList(list);

    cout << "After clear: ";
    checkList(list);

    // Автоматичні тести
    runTests();

    // Звіт про пам'ять
    cout << "\nMEMORY REPORT" << endl;
    cout << "Allocated nodes: " << allocations << endl;
    cout << "Freed nodes: " << deallocations << endl;
    cout << "Active nodes: "
        << allocations - deallocations << endl;

    cout << "Final list state: "
        << (isEmpty(list) ? "empty" : "not empty") << endl;

    return 0;
}