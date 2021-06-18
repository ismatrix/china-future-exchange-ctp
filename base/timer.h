/////////////////////////////////////////////////////////////////////////
///@system 定时器系统
///@company 
///@file 
///@brief 处理事件相关的定时任务框架
///@history 
///20160326	dreamyzhang
/////////////////////////////////////////////////////////////////////////

#ifndef	 __TIMER__H__
#define	 __TIMER__H__

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

//nw_timer timer
//timer.set(2.0, this, &callback_member, NULL, true);
//timer.__call()
//
//timer.set(2.0,  &callback_static, NULL, true);
//需要在线程主循环中做些操作

class nw_timer
{
	class handler_t { };
	typedef nw_timer self_t;
	typedef void (handler_t::*handle_member_func_t)(self_t&);
	typedef void (*handle_func_t)(self_t&);
public:
	inline nw_timer():_hdlr(NULL), _arg(NULL), _repeat(false), _hh(-1), _mm(-1){ memset(&_cb, 0, sizeof(_cb)); _tv_front =0; _exec=false; _wait=0;}
	~nw_timer() { }
	/**
	 * initialize the timer.
	 * @param tv		the timer repeat time value
	 * @param handler	the callback method will be called on which class object
	 * @param function	the callback method
	 * @param arg		an argument carried by the timer
	 * @param repeat	whether this timer will repeat infinitely
	 * @param wait      the time for waitting  on the first exec.
	 */
	template<typename handler_type, typename function_type>
		inline void set(double tv, handler_type* handler, function_type function, void* arg, bool repeat, int hh=-1, int mm=-1, uint32_t wait=0)
		{
			real_set(tv, (handler_t*)handler, (handle_member_func_t)function, arg, repeat, hh, mm, wait);
		}
	template<typename function_type>
		inline void set(double tv, function_type function, void* arg, bool repeat, int hh=-1, int mm=-1, uint32_t wait=0)
		{
			real_set(tv, (handle_func_t)function, arg, repeat, hh, mm, wait);
		}

	template<typename handler_type, typename function_type>
		inline void handle(handler_type* handler, function_type function)
		{
			_hdlr = (handler_t*)handler;
			_cb._member_function = (handle_member_func_t)function;
		}
	template<typename function_type>
		inline void handle(function_type function)
		{
			_hdlr = NULL;
			_cb._static_function = (handle_func_t)function;
		}
	inline void arg(void* p) { _arg = p; }
	inline void* arg(void) const { return _arg; }
	inline void repeat(bool b) { _repeat = b; }
	inline bool repeat() { return _repeat; }

	///在主线程里面执行
	inline void __call()
	{
		if(!_repeat && _exec) return;

		if(_hh != -1)
		{
			struct tm now;
			time_t t= time(NULL);
			localtime_r(&t, &now);
			if(_exec || now.tm_hour < _hh ||  (now.tm_hour == _hh && now.tm_min <= _mm))
			{
				return ;
			}
		}
		
		if(NULL!=_hdlr)
			(_hdlr->*(_cb._member_function))(*this);
		else
			(*(_cb._static_function))(*this);
		_exec = true;
	}

	inline void check()
	{
		if(!_repeat && _exec) return;

		struct timeval tv;
		gettimeofday(&tv, NULL);
		uint64_t t = tv.tv_usec + tv.tv_sec*1000000;
		if(_tv_front < 1) _tv_front = t;

		if(!_exec && t - _tv_front < _wait) return;

		if(t - _tv_front >=  (uint64_t)(_tv * 1000000))
		{
			__call();
			_tv_front = t;
		}
	}
	void set_fail() { _exec = false;}
	void set_succ() { _exec = true;}
	void reset()
	{
		
	}
private:
	inline void real_set(double tv, handler_t* handler, handle_member_func_t member_function, void* arg, bool repeat, int hh, int mm, uint32_t wait)
	{
		_tv = tv;
		_hdlr = handler;
		_cb._member_function = member_function;
		_arg = arg;
		_repeat = repeat;
		_wait = wait;
		_hh = hh;
		_mm = mm;
	}
	inline void real_set(double tv, handle_func_t static_function, void* arg, bool repeat, int hh, int mm, uint32_t wait)
	{
		_tv = tv;
		_hdlr = NULL;
		_cb._static_function = static_function;
		_arg = arg;
		_repeat = repeat;
		_wait = wait;
		_hh = hh;
		_mm = mm;
	}

private:
	uint64_t _tv_front;		//上一次执行时间
	bool	 _exec;			//执行标志
	uint32_t _wait;			//第一次等待时长

	double _tv;				//定时器间隔
	handler_t* _hdlr;
	union
	{
		handle_func_t _static_function;
		handle_member_func_t _member_function;
	}_cb;
	void* _arg;
	
	bool _repeat;
	bool _running;

	/******************固定时间执行**************************/
	int _hh;
	int _mm;

};


#endif





