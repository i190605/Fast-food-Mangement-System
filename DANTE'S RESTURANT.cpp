
  // DANTE'S RESTURANT//
  //.by HAMZA TARIQ (19I-0605).

  // LIBRARIES
  #include <semaphore.h>
  #include <unistd.h>
  #include <sys/wait.h>
  #include <pthread.h>
  #include <iostream>
  #include <cstring>
  #define csno 1
  using namespace std;
  // INITIALIZATION
  int fd_comm[2];
  int fd_B[2];
  int fd_C[2];
  int fd_D[2];
  int fd_E[2];
  int total_bill;
  int z;
  int Daily_sales;
  int w_sales;
  
  pthread_t thread1[csno];
  pthread_t thread2[csno];
  pthread_t thread3;
  sem_t a_sm, a_sm2, a_sm3;
  struct D_REST
  {
    string assig_order;
    D_REST* pushptr;
  };
  class Order
  {
  private:
    D_REST* ptr_front;
  public:
    Order(void) { ptr_front = NULL; }
    void push_order(string);
    string cok_order(void);
    void show_orders(void);
  };
  void Order::push_order(string assig_order)
  {
    D_REST* newassig_order = new D_REST;
    newassig_order->assig_order = assig_order;
    newassig_order->pushptr = NULL;
    if (ptr_front == NULL) {
      ptr_front = newassig_order;
    } else {
      D_REST* ptr_curr = ptr_front;

      if (ptr_curr != NULL) {
        ptr_curr = ptr_curr->pushptr;
      }
      ptr_curr = newassig_order;
    }
  }
  string Order::cok_order()
  {
    string assig_order = ptr_front->assig_order;
    ptr_front = ptr_front->pushptr;
    return assig_order;
  }
  void Order::show_orders()
  {
    D_REST* ptr_curr = ptr_front;
    while (ptr_curr != NULL) {
      cout << ptr_curr->assig_order << endl;
      ptr_curr = ptr_curr->pushptr;
    }
  }

  void Manager()
  { Order assig_orders;
    string Menu = "===================================DANTE 'S ALA "
                  "CARTE===========================\n\n\t\a\n |\tA) "
                  "Pizza|\n\n "
                  "|\tB)ZINGER| \n\n |\tC)SOFT DRINK|\n";
    close(fd_comm[1]);
    char buff[100] = { '\0' };
    read(fd_comm[0], buff, sizeof(buff));
    cout << buff << endl;
    // STEP2The manager will give the menu to the customer.
    cout << "\nManager Giving Menu To consumer!\n\n";
    close(fd_B[0]);
    char* disp_a = const_cast<char*>(Menu.c_str());
    write(fd_B[1], disp_a, strlen(disp_a));
    for (int i = 0; i < 50; i++) {
      buff[i] = '\0';
    }
    read(fd_comm[0], buff, sizeof(buff));
    cout << buff << endl;
    // Step-4: The manager will receive an order and keep these orders in the
    // sequence
    assig_orders.push_order(buff);
    
    // Step-5: The manager will calculate the bill of the customer and add into
    // (Daily_Sales).
    char* assig_order = const_cast<char*>(assig_orders.cok_order().c_str());
    close(fd_C[0]);
    write(fd_C[1], assig_order, strlen(assig_order));
    close(fd_C[1]);
    close(fd_comm[0]);
    close(fd_B[0]);
  }

  void* cheif(void*)
  {
    
    close(fd_C[1]);
    
    char buff[100] = { '\0' };
    
    read(fd_C[0], buff, sizeof(buff));
    int wait_order = 0;
    // Step-6: Now any available cook will get the order
    for (int i = 0; i < strlen(buff); i++) {
      sleep(2);
      if (buff[i] == 'A') {
        wait_order += 5;
        total_bill += 20;
      } else if (buff[i] == 'B') {
        wait_order += 5;
        total_bill += 20;
      } else if (buff[i] == 'C') {
        wait_order += 1;
        total_bill += 1;
      }
      cout << "========BILL==========" << endl;
      cout << "--PLEASE PAY THE CUREENT BILL WHILE WE ARE  PROCESSING YOUR ORDER "
              "----"
          << endl;
      int gst = total_bill = total_bill + 50;
      cout << " \t Total bill including taxes  =" << total_bill
          << "INCLUSION OF ALL SERVICE CHARGES =" << gst << endl;
            w_sales += total_bill;
             Daily_sales += total_bill;
              cout << "\n\n\nTOTAL WAITER  SALES" << w_sales<<endl;}
      sleep(2);
    
    // Step-7: One order can have multiple dishes therefore multiple cooks can
    // work for the preparation of one order to minimize preparation time.
    sem_post(&a_sm2);
    sem_post(&a_sm);
    cout << "Preparing ORDER!\n";
    sleep(wait_order);
    Daily_sales += total_bill;
    
   
    
    close(fd_D[0]);
    write(fd_D[1], buff, strlen(buff));
    close(fd_D[1]);
    close(fd_C[0]);
    pthread_exit(NULL);
    
  }

  void* waiter(void*)
  {
    close(fd_D[1]);
    char buff[100] = { '\0' };
    read(fd_D[0], buff, sizeof(buff));
    cout << "\n ORDER IS PROCESSING AND WAITER IS FETCHING IT " << endl;
    // Step-8: Any available waiters will get prepared food and deliver to
    // customers one waiter can deliver one order at a time. After delivery waiter
    //
    cout << "CONFIRMED ORDER IS "<<buff << endl;
    // will add order amount in (Daily_Waiter_Sales).
    close(fd_E[0]);
    write(fd_E[1], buff, strlen(buff));
    close(fd_E[1]);
    close(fd_D[0]);
    pthread_exit(NULL);
  }


  void* consumer(void*)
  {
    cout << "===================================" << endl;
    cout << "==Welcome to Dante 's Resturant====" << endl;
    cout << "===================================" << endl;
    cout << endl;
    cout << "Welcome NEW CUSTOMER" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    close(fd_comm[0]);
    string str_a;
    // STEP 1 :Customer will tell his name and ID to Manager and can ask him about
    // Menu.
    cout << "INPUT CUSTOMER NAME: ";
    cin >> str_a;
    string buf = "AOA !  I am  MR/MS  " + str_a + " and my  Customer id is: " + to_string(getpid()) + " can i have your resturant  Menu ?"; // assigning the customer
                                                      // id as the cus_id

    char* disp_a = const_cast<char*>(buf.c_str());

    write(fd_comm[1], disp_a, strlen(disp_a));
    close(fd_B[1]);
    char buff[500] = { '\0' };
    read(fd_B[0], buff, sizeof(buff));
    cout << buff << endl;
    // step 3: Customer will check menu and place order.
    string assig_order;
    cout << "===================Keys========== \n";
    cout << "Press (A) for Pizza  -$20\n";
    cout << "Press (B) for Zinger Burger-$20 \n";
    cout << "Press (C) for SOFT DRINK  $1\n";
    cout << "------------------\n|PROCESSING TIME|\n-----------------\n|PIZZA =5 "
            "SECS |\n|BURGER "
            "=3SEC  |\n|SOFTDRINK=1SEC"
        << endl;
    cout << "\n\n\n\tYou can order multiple items as ORDER1,ORDER2\t\t\t\n"
        << endl;
    cout << "Noting down your order" << endl;
    cin >> assig_order;
    disp_a = const_cast<char*>(assig_order.c_str());
    write(fd_comm[1], disp_a, strlen(disp_a));
    // Step-9: Customer will get his order give confirmation message and
    // terminate.
    sem_wait(&a_sm);
    cout << "\n\n\aT\aerminating Thats it\a\a";
    close(fd_E[1]);
    for (int i = 0; i < 1000; i++) {
      buff[i] = '\0';
    }
    sem_post(&a_sm);
    read(fd_E[0], buff, sizeof(buff));
    close(fd_E[0]);
    close(fd_comm[1]);
    close(fd_B[0]);
    pthread_exit(NULL);
  }
  int main()
  {

    sem_init(&a_sm, 1, 1);
    sem_init(&a_sm2, 1, 1);
    pipe(fd_B);
    pipe(fd_E);
    pipe(fd_comm);
    pid_t cus_id = fork();
    
    /*
       pthread_mutex_init(&mutex1, NULL);

    pthread_create(&thread[0], NULL, cheif, NULL);

    for(int i = 1; i < 3; i++){
        pthread_create(&thread[i], NULL, waiter, NULL);
    }

    pthread_mutex_destroy(&mutex1);
    pthread_exit(NULL);
    */
    if (cus_id > 0) {
      pipe(fd_C);
      pid_t pid_b = fork();
      if (pid_b > 0) {
        Manager();
      } else if (pid_b == 0) {
        pipe(fd_D);
        pid_t pid_c = fork();
        if (pid_c > 0) {
          for (int i = 0; i < csno; i++) {
            pthread_create(&thread2[i], NULL, cheif, NULL);
            pthread_join(thread3, NULL);
          }
        } else if (pid_c == 0) {
          pthread_create(&thread3, NULL, waiter, NULL);
          sleep(2);
        }
      }
      wait(NULL);
    } else if (cus_id == 0) {
      for (int i = 0; i < csno; i++) {
        pthread_create(&thread1[i], NULL, consumer, NULL);
        pthread_join(thread2[i], NULL);
        sleep(5);
      }
    } else if (cus_id < 0)
      printf(" Error opening the file \n ");
    pthread_exit(NULL);
  }

  // Step-10: At the end you have to compare Daily_Waiter_Sales with Daily_Sales
  // and print their values.
