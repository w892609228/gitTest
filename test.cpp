// main.cpp
#include <gtest/gtest.h>
#include <pthread.h>

class testGlobal: public testing::Environment
{
    virtual void SetUp(){
        std::cout<<"Enter testGlobal SetUp"<<std::endl;
    }
    virtual void TearDown(){
        std::cout<<"Enter testGlobal TearDown"<<std::endl;
    }
};

class testdemo: public testing::Test
{
protected:
    static void SetUpTestCase(){
        std::cout<<"Enter SetUpTestCase"<<std::endl;
    }
    static void TearDownTestCase(){
        std::cout<<"ERnter TearDownTestCase"<<std::endl;
    }
    virtual void SetUp(){
        std::cout<<"Enter SetUp"<<std::endl;
        index = 1;
    }
    virtual void TearDown(){
        std::cout<<"Enter TearDown"<<std::endl;
    }

    int getIndex(){
        return index;
    }
    void setIndex(int idx)
    {
        index = idx;
    }
private:
    int index;
};
/*class testparam: public testing::TestWithParam<bool>
{
    
};
TEST_P(testparam, setIndex)
{
    bool n = GetParam();
    std::cout<<"n = "<<n<<std::endl;
}
std::vector<int> vtr ={1,2,3,4,5,6};
INSTANTIATE_TEST_SUITE_P(TrueReturn, testparam, testing::Bool());*/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZR;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
void* add(void *arg)
{
    int num = 0;
    int item = *(int*)arg;
    while(true)
    {
        num++;
        pthread_mutex_lock(&mutex);
        std::cout<<"enter add"<<std::endl;
        pthread_cond_signal(&cond2);
        pthread_cond_wait(&cond, &mutex);
        std::cout<<"sum:"<<item<<std::endl;
        pthread_mutex_unlock(&mutex);
        if(num == 10)
            break;
    }
    

    return NULL;
}

void* add2(void *arg)
{
    int num = 0;
    int item = *(int*)arg;
    while(true)
    {
        num++;
        pthread_mutex_lock(&mutex);
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex);
        std::cout<<"sum2:"<<item<<std::endl;
        pthread_mutex_unlock(&mutex);
        if(num == 10)
            break;
    }
    

    return NULL;
}

/*TEST(test, test_add)
{
    EXPECT_EQ(3, add(1, 2));
}
TEST(testdemo1, test_add)
{
    EXPECT_EQ(3, add(1, 2));
    ASSERT_EQ(6, add(3,3));
}
TEST(testdemo1, test_add2)
{
    EXPECT_EQ(6, add(3,3));
}*/
/*TEST_F(testdemo, printIndex1)
{
    EXPECT_EQ(1, getIndex());
}
TEST_F(testdemo, printIndex2)
{
    setIndex(6);
    EXPECT_EQ(6, getIndex());
}*/
void threadProcess(){
    static int arg=6;
    pthread_t trdId;
    pthread_create(&trdId, NULL, add, &arg);
    pthread_t tid2;
    pthread_create(&tid2, NULL, add2, &arg);
    pthread_join(trdId, NULL);
    pthread_join(tid2,NULL);
}

int main(int argc, char ** argv)
{
    testing::AddGlobalTestEnvironment(new testGlobal);
    testing::InitGoogleTest(&argc, argv);
    pthread_mutex_init(&mutex, NULL);
    threadProcess();
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    std::cout<<"End main"<<std::endl;
    return RUN_ALL_TESTS();
}