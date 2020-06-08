#include "gtest/gtest.h"

extern "C"
{
    #include "currentCondition.h"
}

class CurrentConditionTest : public ::testing::Test
{
protected:
    currentCondition_t condition;
    void SetUp() override
    {
        condition = currentCondition_create();
    }

    void TearDown() override
    {

    }
};

TEST_F(CurrentConditionTest, Test_Humidity_On_Create) {
    uint16_t humidity = currentCondition_getHumidity(condition);
    ASSERT_EQ(humidity, 0);
}

TEST_F(CurrentConditionTest, Test_Humidity_Valid) {
    currentCondition_setHumidity(condition, 50);
    uint16_t humidity = currentCondition_getHumidity(condition);
    ASSERT_EQ(humidity, 50);
}

TEST_F(CurrentConditionTest, Test_Humidity_Invalid) {
    currentCondition_setHumidity(condition, -10);
    uint16_t humidity = currentCondition_getHumidity(condition);
    ASSERT_NE(humidity, -10);
    ASSERT_EQ(humidity, 0);
}

TEST_F(CurrentConditionTest, Test_Humidity_Lower_Bound) {
    currentCondition_setHumidity(condition, 0);
    uint16_t humidity = currentCondition_getHumidity(condition);
    ASSERT_EQ(humidity, 0);
}

TEST_F(CurrentConditionTest, Test_Humidity_Above_Lower_Bound) {
    currentCondition_setHumidity(condition, 9);
    uint16_t humidity = currentCondition_getHumidity(condition);
    ASSERT_EQ(humidity, 9);
}

TEST_F(CurrentConditionTest, Test_Humidity_Upper_Bound) {
    currentCondition_setHumidity(condition, 1000);
    uint16_t humidity = currentCondition_getHumidity(condition);
    ASSERT_EQ(humidity, 1000);
}

TEST_F(CurrentConditionTest, Test_Humidity_Below_Upper_Bound) {
    currentCondition_setHumidity(condition, 990);
    uint16_t humidity = currentCondition_getHumidity(condition);
    ASSERT_EQ(humidity, 990);
}

//-------------------------------------------------------------

TEST_F(CurrentConditionTest, Test_Temperature_On_Create) {
    int16_t temperature = currentCondition_getTemperature(condition);
    ASSERT_EQ(temperature, 0);
}

TEST_F(CurrentConditionTest, Test_Temperature_Invalid) {
    currentCondition_setTemperature(condition, -500);
    int16_t temperature = currentCondition_getTemperature(condition);
    ASSERT_NE(temperature, -500);
    ASSERT_EQ(temperature, 0);
}

TEST_F(CurrentConditionTest, Test_Temperature_Lower_Bound) {
    currentCondition_setTemperature(condition, -400);
    int16_t temperature = currentCondition_getTemperature(condition);
    ASSERT_EQ(temperature, -400);
}

TEST_F(CurrentConditionTest, Test_Temperature_Above_Lower_Bound) {
    currentCondition_setTemperature(condition, -350);
    int16_t temperature = currentCondition_getTemperature(condition);
    ASSERT_EQ(temperature, -350);
}

TEST_F(CurrentConditionTest, Test_Temperature_Upper_Bound) {
    currentCondition_setTemperature(condition, 550);
    int16_t temperature = currentCondition_getTemperature(condition);
    ASSERT_EQ(temperature, 550);
}

TEST_F(CurrentConditionTest, Test_Temperature_Below_Upper_Bound) {
    currentCondition_setTemperature(condition, 500);
    int16_t temperature = currentCondition_getTemperature(condition);
    ASSERT_EQ(temperature, 500);
}

//---------------------------------------------------------------------

TEST_F(CurrentConditionTest, Test_CO2_On_Create) {
    uint16_t CO2 = currentCondition_getCO2Data(condition);
    ASSERT_EQ(CO2, 0);
}

TEST_F(CurrentConditionTest, Test_CO2_Invalid) {
    currentCondition_setCO2(condition, 150);
    uint16_t CO2 = currentCondition_getCO2Data(condition);
    ASSERT_NE(CO2, 150);
    ASSERT_EQ(CO2, 0);
}

TEST_F(CurrentConditionTest, Test_CO2_Lower_Bound) {
    currentCondition_setCO2(condition, 200);
    uint16_t CO2 = currentCondition_getCO2Data(condition);
    ASSERT_EQ(CO2, 200);
}

TEST_F(CurrentConditionTest, Test_CO2_Above_Lower_Bound) {
    currentCondition_setCO2(condition, 250);
    uint16_t CO2 = currentCondition_getCO2Data(condition);
    ASSERT_EQ(CO2, 250);
}

TEST_F(CurrentConditionTest, Test_CO2_Upper_Bound) {
    currentCondition_setCO2(condition, 10000);
    uint16_t CO2 = currentCondition_getCO2Data(condition);
    ASSERT_EQ(CO2, 10000);
}

TEST_F(CurrentConditionTest, Test_CO2_Below_Upper_Bound) {
    currentCondition_setCO2(condition, 9900);
    uint16_t CO2 = currentCondition_getCO2Data(condition);
    ASSERT_EQ(CO2, 9900);
}

//-----------------------------------------------------------

TEST_F(CurrentConditionTest, Test_Sound_On_Create) {
    uint16_t sound = currentCondition_getSound(condition);
    ASSERT_EQ(sound, 0);
}

TEST_F(CurrentConditionTest, Test_Sound_Invalid) {
    currentCondition_setSound(condition, -50);
    uint16_t sound = currentCondition_getSound(condition);
    ASSERT_NE(sound, -50);
    ASSERT_EQ(sound, 0);
}

TEST_F(CurrentConditionTest, Test_Sound_Lower_Bound) {
    currentCondition_setSound(condition, 0);
    uint16_t sound = currentCondition_getSound(condition);
    ASSERT_EQ(sound, 0);
}

TEST_F(CurrentConditionTest, Test_Sound_Above_Lower_Bound) {
    currentCondition_setSound(condition, 30);
    uint16_t sound = currentCondition_getSound(condition);
    ASSERT_EQ(sound, 30);
}

TEST_F(CurrentConditionTest, Test_Sound_Upper_Bound) {
    currentCondition_setSound(condition, 150);
    uint16_t sound = currentCondition_getSound(condition);
    ASSERT_EQ(sound, 150);
}

TEST_F(CurrentConditionTest, Test_Sound_Below_Upper_Bound) {
    currentCondition_setSound(condition, 120);
    uint16_t sound = currentCondition_getSound(condition);
    ASSERT_EQ(sound, 120);
}