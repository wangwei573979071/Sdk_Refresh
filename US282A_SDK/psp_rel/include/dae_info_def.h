/********************************************************************************
 *                              GL5116
 *                            Module: music_player
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fiona    2015-01-27 15:00     1.0             build this file
 ********************************************************************************/

#ifndef _DAE_PARA_H
#define _DAE_PARA_H

#include <typeext.h>
#include <attr.h>

//支持多频点多频段开关
#define SUPPORT_MULTI_FREQ_MULTI_BAND_SETTING   1
#define EQ_NUM_BANDS 20   //NOTE!! 请同步到 mmm_dae_def.h 宏定义 MAX_PEQ_SEG

struct eq_band_setting    //NOTE!! 请同步到 mmm_dae_def.h 结构体定义 peq_band_t
{
    short cutoff;   //中心频率，20:1:24000Hz
    short q;        //0.3:0.1:3，传值扩大10倍，即范围-3:1:30
    short gain;     //-20:0.1:12dB，传值扩大10倍，即范围-200:1:120
    short type;     ///*滤波器类型：0是无效，1是常规EQ滤波器，2是高通滤波器，其它值为reserve*/
};

#define MDRC_NUM_BANDS 3  //NOTE!! 请同步到 mmm_dae_def.h 宏定义 MAX_MDRC_SEG

struct mdrc_band_setting  //NOTE!! 请同步到 mmm_dae_def.h 结构体定义 mdrc_band_t
{
    short threshold;
    short attack_time;
    short release_time;
    short reserve;//确保4字节对齐
};

typedef struct
{
    short enable;				            //default: 200ms,step 0.01ms, set 200*100 = 20000    
    short DownExp_attack_time;				//default: 200ms,step 0.01ms, set 200*100 = 20000
	short DownExp_release_time;				//default: 10ms,step 1ms, set 10
	short DownExp_set0_threshold;		    //default: 1ms, step 0.01ms, set 100;
	short DownExp_threshold;			    //default: -80dB(min), step 0.1dB, set -800
	short DownExp_ratio;				    //default: 3(greater than 1)  
}dae_noise_reduction_t;

#if (SUPPORT_MULTI_FREQ_MULTI_BAND_SETTING == 1)
#define MFBE_BANDS     10
#define MAX_FREQ_POINT 10
#endif

typedef struct
{
    int sample_rate;            //采样率，支持44100,48000Hz两种
    short channels;               //目前都是按双通道处理
    short block_size;             //帧长，目前为128点长度
	
    int   fade_handle;            //淡入淡出处理句柄，如果加载的音效库不支持淡入淡出，其值为淡入淡出函数的地址，如果支持其为NULL
    short fade_in_time_ms;        //淡入长度，[50 100 200 300 400 500]ms
    short fade_out_time_ms;       //淡出长度，[50 100 200 300 400 500]ms
    short mute_time_ms;           //静音长度，长度没有限制，单位ms
    short fade_in_flag;           //淡入标志位，置1表示下一帧开始淡入
    short fade_out_flag;          //淡出标志位，置1表示下一帧开始淡出
    short mute_flag;              //静音标志位，置1表示下一帧开始静音
	
    short DAE_init_flag;          //初始化标识位
    short DAE_change_flag;        //DAE全部参数改变标识位
    short output_channel_config;//1:左右声道交换 2: 单端耳机应用(L=R=原始的(L/2 + R/2)), 0 or 其它值, 跟原始的左右声道一致；该功能与音效无关，不受 BYPASS 影响
    short bypass;               //bypass,1表示直通，0表示DAE处理
    short precut;                 //预衰减，初始化为0；precut 不受 BYPASS 影响
	/***********************************/

//确保4字节对齐
    short Vbass_enable;         //Vbass开关
    short Surround_enable;      //Surround开关
    short TrebleEnhance_enable; //Treble开关
    short Peq_enable;           //Peq开关
    short SpeakerCompensation_enable;//SpeakerCompensation开关
    short Limiter_enable;       //Limiter开关
    short MultibandDRC_enable;  //MDRC开关
    short DownwardExpander_enable; //downward expander(noise gate)开关	//确保4字节对齐
    /***********************************/
#if (SUPPORT_MULTI_FREQ_MULTI_BAND_SETTING == 1)    
    short MultiFreqBandEnergy_enable;//多频段能量计算开关
    short FreqSpetrumDisplay_enable;//多频点能量计算开关
#endif  

    /*******for noise reduction*********/
	short DownExp_attack_time;				//default: 100ms,step 0.01ms, set 10*100 = 1000
	short DownExp_release_time;				//default: 10ms,step 1ms, set 10
	short DownExp_set0_threshold;				//default: 5, step 1, set 5;表示采样值在该值以下直接清0
	short DownExp_threshold;					//default: -75dB(min), step 0.1dB, set -750
	short DownExp_ratio;						//default: 5(greater than 1)

//确保4字节对齐
    /***********************************/
	short Vbass_low_cut_off_frequency;
	short Vbass_gain;
    int Vbass_type;             //虚拟低音的类型
    /***********************************/
	short Surround_angle;
	short Surround_gain;
    /***********************************/
	short Treble_frequency;
	short Treble_gain;
    /***********************************/
    //int MDRC_change_flag;     //MDRC单独的参数改变标识位，MDRC参数改变时置1，同时DAE_change_flag置1
    int crossover_freqency[MDRC_NUM_BANDS-1];
    short MDRC_compensation_peak_detect_attack_time;	//default: 0.5ms, step: 0.01ms, setting: 5*10
	short MDRC_compensation_peak_detect_release_time;	//default: 500ms, step: 1ms, setting: 500
	short MDRC_compensation_threshold;					//default: -100,step 0.1dB,setting: -10*10 = -100
	short MDRC_compensation_filter_Q;					//default: 1,step 0.1, setting:0.1*10 = 1, 0.1~0.9
    /***********************************/
    int Peq_change_flag;        //peq单独的参数改变标识位，peq参数改变时置1，同时DAE_change_flag置1
    struct eq_band_setting band_settings[EQ_NUM_BANDS]; //peq频点参数，当band_settings中四个参数中有改变时Peq_change_flag、DAE_change_flag同时置1
    short band_enable_type[EQ_NUM_BANDS]; //peq类型和enable选项，1表示Speaker PEQ，2表示Post PEQ，0表示不执行
    /***********************************/
    short SpeakerCompensationChangeFlag;
    short FilterOrder;
    short FilterQvalLeft;
    short FilterQvalRight;
    /***********************************/
    int makeup_gain;            //makeup gain，将信号提升到0DB，以降低音量，提升信噪比
    
#if (SUPPORT_MULTI_FREQ_MULTI_BAND_SETTING == 1)
    /**************Multi-freq band energy*********************/
    short duration_ms;                      //统计时长，单位ms
    short num_band;                         //频段数,算法内部最多支持10段
	short f_c[MFBE_BANDS];					//带通中心频率
	short energys[MFBE_BANDS];				//计算出来的频段能量，供方案端读!!!
	/**************freq spetrum display*********************/
	int num_freq_point;						//频点数,算法内部最多支持12点
	short freq_point[MAX_FREQ_POINT];			//各频点数组
	short freq_point_mag[MAX_FREQ_POINT];		//计算出来的频点能量，供方案端读!!!
#endif
 
    /***********************************/
    short energy_detect_enable; //信号能量检测使能
    short period;               //统计周期时长，以1ms为单位
    short period_count;         //有效周期数
    short preadjust_db;         //信号大小降低较多才启动预调机制，以0.1db为单位
    short preadjust_count;      //开始预调降低信号大小的周期数，然后每个周期降低相应比例
//signal_energy_inner必须放在energy前一个位置
    short signal_energy_inner;  //！！方案端去读该值！！ 切换信号检测时存放初始值
    /***********************************/
    short  energy;     //解码帧pcm平均值
    short  energy_max; //解码帧pcm绝对值最大值
//以上3个成员必须紧挨着
	
	/***********************************/
	short Limiter_threshold;
	short Limiter_attack_time;
	short Limiter_release_time;
	short reserve;
	
	struct mdrc_band_setting mdrc_band_settings[MDRC_NUM_BANDS];
	
}DAE_para_info_t;

extern DAE_para_info_t g_dae_param_info;

#endif //_DAE_PARA_H
