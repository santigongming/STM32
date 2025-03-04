#include "PID.h"
#include "protocol.h"
/**
  * @brief  PID算法实现
  * @param  actual_val:实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
  
/*定义位置PID与速度PID结构体型的全局变量*/
extern PID pid_location;
extern PID pid_speed;

/**
  * @brief  位置PID算法实现
  * @param  actual_val:实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
#define LOC_DEAD_ZONE 60 /*位置环死区*/
#define LOC_INTEGRAL_START_ERR 200 /*积分分离时对应的误差范围*/
#define LOC_INTEGRAL_MAX_VAL 800   /*积分范围限定，防止积分饱和*/
float location_pid_realize(PID *pid, float actual_val)
{
	/*计算目标值与实际值的误差*/
	pid->err = pid->target_val - actual_val;

	/* 设定闭环死区 */
	if((pid->err >= -LOC_DEAD_ZONE) && (pid->err <= LOC_DEAD_ZONE))
	{
		pid->err = 0;
		pid->integral = 0;
		pid->err_last = 0;
	}

	/*积分项，积分分离，偏差较大时去掉积分作用*/
	if(pid->err > -LOC_INTEGRAL_START_ERR && pid->err < LOC_INTEGRAL_START_ERR)
	{
		pid->integral += pid->err;  
        /*积分范围限定，防止积分饱和*/
		if(pid->integral > LOC_INTEGRAL_MAX_VAL)
		{
			pid->integral = LOC_INTEGRAL_MAX_VAL;
		}
		else if(pid->integral < -LOC_INTEGRAL_MAX_VAL)
		{
			pid->integral = -LOC_INTEGRAL_MAX_VAL;
		}
	}	

	/*PID算法实现*/
	pid->output_val = pid->Kp * pid->err +
	                  pid->Ki * pid->integral +
	                  pid->Kd * (pid->err - pid->err_last);

	/*误差传递*/
	pid->err_last = pid->err;

	/*返回当前实际值*/
	return pid->output_val;
}


/**
  * @brief  速度PID算法实现
  * @param  actual_val:实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
#define SPE_DEAD_ZONE 5.0f /*速度环死区*/
#define SPE_INTEGRAL_START_ERR 100 /*积分分离时对应的误差范围*/
#define SPE_INTEGRAL_MAX_VAL 260   /*积分范围限定，防止积分饱和*/
float speed_pid_realize(PID *pid, float actual_val)
{
	/*计算目标值与实际值的误差*/
	pid->err = pid->target_val - actual_val;

	/* 设定闭环死区 */
	if( (pid->err>-SPE_DEAD_ZONE) && (pid->err<SPE_DEAD_ZONE ) )
	{
		pid->err = 0;
		pid->integral = 0;
		pid->err_last = 0;
	}

#if 0
	/*积分项*/
	pid->integral += pid->err;
#else	
	/*积分项，积分分离，偏差较大时去掉积分作用*/
	if(pid->err > -SPE_INTEGRAL_START_ERR && pid->err < SPE_INTEGRAL_START_ERR)
	{
		pid->integral += pid->err;  
        /*积分范围限定，防止积分饱和*/
		if(pid->integral > SPE_INTEGRAL_MAX_VAL)
		{
			pid->integral = SPE_INTEGRAL_MAX_VAL;
		}
		else if(pid->integral < -SPE_INTEGRAL_MAX_VAL)
		{
			pid->integral = -SPE_INTEGRAL_MAX_VAL;
		}
	}	
#endif
	
	/*PID算法实现*/
	pid->output_val = pid->Kp * pid->err +
	                  pid->Ki * pid->integral +
	                  pid->Kd *(pid->err - pid->err_last);

	/*误差传递*/
	pid->err_last = pid->err;

	/*返回当前实际值*/
	return pid->output_val;
}





/*-----------------------外部链接部分----------------------*/
/**
  * @brief  PID参数初始化
  *	@note 	无
  * @retval 无
  */
void PID_Init()
{
	/* 位置相关初始化参数 */
	pid_location.target_val = 100.0;				
	pid_location.output_val = 0.0;
	pid_location.err = 0.0;
	pid_location.err_last = 0.0;
	pid_location.integral = 0.0;

	pid_location.Kp = 0.05;
	pid_location.Ki = 0;
	pid_location.Kd = 0;

	/* 速度相关初始化参数 */
	pid_speed.target_val=10.0;				
	pid_speed.output_val=0.0;
	pid_speed.err=0.0;
	pid_speed.err_last=0.0;
	pid_speed.integral=0.0;

	pid_speed.Kp = 80.0;
	pid_speed.Ki = 2.0;
	pid_speed.Kd = 100.0;

}

