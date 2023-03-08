#include "clock.h"

void initialize_clock(void)
{
    MySpi1_Rtc_Init();
	MySpi1_Rtc_Read_Time(bcd_time.buffer);
	MySpi1_Rtc_Read_Date(bcd_date.buffer);
    // convert bcd_time values into int_time
}

uint8_t get_milliseconds(void)
{
    // should return int_time milliseconds
	MySpi1_Rtc_Read_Time(bcd_time.buffer);
    return bcd_time.milliseconds;
}

uint8_t get_seconds(void)
{
    // should return int_time seconds
	MySpi1_Rtc_Read_Time(bcd_time.buffer);
    return bcd_time.seconds;
}

uint8_t get_minutes(void)
{
    // should return int_time minutes
	MySpi1_Rtc_Read_Time(bcd_time.buffer);
    return bcd_time.minutes;
}
