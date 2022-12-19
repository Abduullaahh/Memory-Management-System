#include <iostream>
using namespace std;

template <typename T>
class SinglyLinkedList
{
    class Node
    {
    public:
        T data;
        Node* next;
        Node(T data)
        {
            this->data = data;
            this->next = NULL;
        }
    };
    Node* head;
    Node* tail;
    int size;

public:
    SinglyLinkedList()
    {
        head = NULL;
        tail = NULL;
        size = 0;
    }

    void Add(T data)
    {
        Node* node = new Node(data);
        if (head == NULL)
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }
        size++;
    }

    void Add(int index, T data)
    {
        if (index < 0 || index > size)
        {
            cout << "Index out of bounds" << endl;
            return;
        }
        Node* node = new Node(data);
        if (index == 0)
        {
            node->next = head;
            head = node;
        }
        else if (index == size)
        {
            tail->next = node;
            tail = node;
        }
        else
        {
            Node* temp = head;
            for (int i = 0; i < index - 1; i++)
            {
                temp = temp->next;
            }
            node->next = temp->next;
            temp->next = node;
        }
        size++;
    }

    T Remove(int index)
    {
        if (index < 0 || index >= size)
        {
            throw "Index out of bounds";
        }
        Node* current = head;
        T data;
        if (index == 0)
        {
            data = head->data;
            head = head->next;
            delete current;
        }
        else if (index == size - 1)
        {
            for (int i = 0; i < index - 1; i++)
            {
                current = current->next;
            }
            data = tail->data;
            delete tail;
            tail = current;
            tail->next = NULL;
        }
        else
        {
            for (int i = 0; i < index - 1; i++)
            {
                current = current->next;
            }
            Node* temp = current->next;
            data = temp->data;
            current->next = temp->next;
            delete temp;
        }
        size--;
        return data;
    }

    T Get(int index)
    {
        if (index < 0 || index >= size)
        {
            throw "Index out of bounds";
        }
        Node* current = head;
        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }
        return current->data;
    }

    int Get_Size()
    {
        return size;
    }

    void Print()
    {
        Node* current = head;
        while (current != NULL)
        {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
};

class Block
{
private:
    int start_byte_id;
    int total_bytes;

public:
    Block()
    {
        start_byte_id = 0;
        total_bytes = 0;
    }
    Block(int start_byte_id, int total_bytes)
    {
        this->start_byte_id = start_byte_id;
        this->total_bytes = total_bytes;
    }

    int Get_Start_Byte_Id()
    {
        return start_byte_id;
    }

    int Set_Total_Bytes()
    {
        return total_bytes;
    }

    void Set_Start_Byte_Id(int start_byte_id)
    {
        this->start_byte_id = start_byte_id;
    }

    void Set_Total_Bytes(int total_bytes)
    {
        this->total_bytes = total_bytes;
    }
};

class Program
{
private:
    int Id;
    int size;
    SinglyLinkedList<Block> blocks;

public:
    Program()
    {
        Id = 0;
        size = 0;
    }
    Program(int Id, int size)
    {
        this->Id = Id;
        this->size = size;
    }

    int Get_Id()
    {
        return Id;
    }

    int Get_Size()
    {
        return size;
    }

    SinglyLinkedList<Block> Get_Blocks()
    {
        return blocks;
    }

    void Set_Id(int Id)
    {
        this->Id = Id;
    }

    void Set_Size(int size)
    {
        this->size = size;
    }

    void Set_Blocks(SinglyLinkedList<Block> blocks)
    {
        this->blocks = blocks;
    }
};

class MemoryMangementSystem
{
    SinglyLinkedList<Program> programs;
    SinglyLinkedList<Block> pool;
    int total_memory_size;
    bool strategy;

public:
    MemoryMangementSystem(int total_memory_size, bool strategy)
    {
        this->total_memory_size = total_memory_size;
        this->strategy = strategy;
        pool.Add(Block(0, total_memory_size));
    }

    void Best_Fit_Strategy(Program program)
    {
        int min = 102400;
        int min_index = -1;
        for (int i = 0; i < pool.Get_Size(); i++)
        {
            if (pool.Get(i).Set_Total_Bytes() >= program.Get_Size() && pool.Get(i).Set_Total_Bytes() < min)
            {
                min = pool.Get(i).Set_Total_Bytes();
                min_index = i;
            }
        }
        if (min_index == -1)
        {
            cout << "Not enough memory for program " << program.Get_Id() << endl;
            return;
        }
        Block block = pool.Get(min_index);
        if (block.Set_Total_Bytes() == program.Get_Size())
        {
            pool.Remove(min_index);
        }
        else
        {
            block.Set_Start_Byte_Id(block.Get_Start_Byte_Id() + program.Get_Size());
            block.Set_Total_Bytes(block.Set_Total_Bytes() - program.Get_Size());
            pool.Add(min_index, block);
        }
        program.Get_Blocks().Add(Block(block.Get_Start_Byte_Id(), program.Get_Size()));
        programs.Add(program);
    }

    void First_Fit_Strategy(Program program)
    {
        int index = -1;
        for (int i = 0; i < pool.Get_Size(); i++)
        {
            if (pool.Get(i).Set_Total_Bytes() >= program.Get_Size())
            {
                index = i;
                break;
            }
        }
        if (index == -1)
        {
            cout << "Not enough memory for program " << program.Get_Id() << endl;
            return;
        }
        Block block = pool.Get(index);
        if (block.Set_Total_Bytes() == program.Get_Size())
        {
            pool.Remove(index);
        }
        else
        {
            block.Set_Start_Byte_Id(block.Get_Start_Byte_Id() + program.Get_Size());
            block.Set_Total_Bytes(block.Set_Total_Bytes() - program.Get_Size());
            pool.Add(index, block);
        }
        program.Get_Blocks().Add(Block(block.Get_Start_Byte_Id(), program.Get_Size()));
        programs.Add(program);
    }

    void Get_Memory(int Id, int size)
    {
        Program program(Id, size);
        if (strategy)
        {
            First_Fit_Strategy(program);
        }
        else
        {
            Best_Fit_Strategy(program);
        }
    }

    void Delete_Program(int Id)
    {
        for (int i = 0; i < programs.Get_Size(); i++)
        {
            if (programs.Get(i).Get_Id() == Id)
            {
                Program program = programs.Remove(i);
                for (int j = 0; j < program.Get_Blocks().Get_Size(); j++)
                {
                    Block block = program.Get_Blocks().Get(j);
                    if (pool.Get_Size() == 0)
                    {
                        pool.Add(block);
                    }
                    else
                    {
                        if (strategy)
                        {
                            if (block.Get_Start_Byte_Id() < pool.Get(0).Get_Start_Byte_Id())
                            {
                                pool.Add(0, block);
                            }
                            else if (block.Get_Start_Byte_Id() > pool.Get(pool.Get_Size() - 1).Get_Start_Byte_Id())
                            {
                                pool.Add(block);
                            }
                            else
                            {
                                for (int k = 0; k < pool.Get_Size() - 1; k++)
                                {
                                    if (block.Get_Start_Byte_Id() > pool.Get(k).Get_Start_Byte_Id() && block.Get_Start_Byte_Id() < pool.Get(k + 1).Get_Start_Byte_Id())
                                    {
                                        pool.Add(k + 1, block);
                                        break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (block.Get_Start_Byte_Id() > pool.Get(0).Get_Start_Byte_Id())
                            {
                                pool.Add(0, block);
                            }
                            else if (block.Get_Start_Byte_Id() < pool.Get(pool.Get_Size() - 1).Get_Start_Byte_Id())
                            {
                                pool.Add(block);
                            }
                            else
                            {
                                for (int k = 0; k < pool.Get_Size() - 1; k++)
                                {
                                    if (block.Get_Start_Byte_Id() < pool.Get(k).Get_Start_Byte_Id() && block.Get_Start_Byte_Id() > pool.Get(k + 1).Get_Start_Byte_Id())
                                    {
                                        pool.Add(k + 1, block);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                cout << "Program " << Id << " has been removed from the memory" << endl;
                return;
            }
        }
        cout << "Program " << Id << " cannot be removed from the memory because it is not in the memory" << endl;
    }

    void Print_Memory()
    {
        cout << "Memory: " << endl;
        for (int i = 0; i < pool.Get_Size(); i++)
        {
            cout << "Block " << i + 1 << ": " << pool.Get(i).Get_Start_Byte_Id() << " - " << pool.Get(i).Get_Start_Byte_Id() + pool.Get(i).Set_Total_Bytes() - 1 << endl;
        }
        cout << endl;
    }
};

void Menu()
{
    system("cls");
    cout << "M E M O R Y - M A N A G E M E N T - S Y S T E M\n\n";
    cout << "\t-----------------------------\n";
    cout << "\t\t   ~Menu~\n";
    cout << "\t-----------------------------\n";
    cout << "\tPress 1 : To Add a Program\n";
    cout << "\tPress 2 : To Give more Memory\n";
    cout << "\tPress 3 : To Remove a Program\n";
    cout << "\tPress 4 : To Print Memory\n";
    cout << "\tPress 0 : To Exit\n";
    cout << "\t-----------------------------\n";
}

int main()
{
    MemoryMangementSystem MMS(102400, 0);
    int size_of_memory = 0, program_ID = 0;
    int opt = -1;
    while (opt != 0)
    {
        Menu();
        cout << "\tEnter your choice : ";
        cin >> opt;
        switch (opt)
        {
        default: { cout << "Enter correct option\n"; system("pause"); }
        case 0:
        {
            return 0;
        }
        case 1:
        {
            cout << "Enter size of program : "; cin >> size_of_memory;
            cout << "Enter ID of program : "; cin >> program_ID;
            MMS.Get_Memory(program_ID, size_of_memory);
            system("pause");
            break;
        }
        case 2:
        {
            cout << "Not Completed\n";
            system("pause");
            break;
        }
        case 3:
        {
            cout << "\tEnter ID of program to remove : "; cin >> program_ID;
            MMS.Delete_Program(program_ID);
            system("pause");
            break;
        }
        case 4:
        {
            MMS.Print_Memory();
            system("pause");
            break;
        }
        }
    }
    return 0;
}