#include <gtest/gtest.h>
extern "C"
{
#include "statemachine.h"
#include "homing.h"
#include "digital_inputs.h"
}

#include "vcd_tracer.hpp"
#include <array>
#include <cmath>
#include <string>
#include <fstream>

TEST(LibCia402, AddFunction) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(-1, 1), 0);
}

TEST(LibCia402, Digital_Inputs)
{
digital_inputs io={0};

    EXPECT_EQ(read_inputs(io),0x0);
    io.negative_limit_switch=true;
    EXPECT_EQ(read_inputs(io),0x1);
    io.positive_limit_switch=true;
    EXPECT_EQ(read_inputs(io),0x3);
}

TEST(LibCia402, GetStatusWord_LowByte)
{
    EXPECT_EQ(get_statusword_lowbyte(drive_state_switch_on_disabled),(1<<6));
    EXPECT_EQ(get_statusword_lowbyte(drive_state_not_ready_to_switch_on),0);
    EXPECT_EQ(get_statusword_lowbyte(drive_state_ready_to_switch_on),0x21);
    EXPECT_EQ(get_statusword_lowbyte(drive_state_switched_on),0x23);
    EXPECT_EQ(get_statusword_lowbyte(drive_state_operation_enabled),0x27);
    EXPECT_EQ(get_statusword_lowbyte(drive_state_quickstop_enabled),0x7);
    EXPECT_EQ(get_statusword_lowbyte(drive_state_fault_reaction_active),0xF);
    EXPECT_EQ(get_statusword_lowbyte(drive_state_fault),0x8);
}

TEST(LibCia402, GetStatusWord_Homing_HighByte)
{
    EXPECT_EQ(get_homing_statusword_highbyte(homing_completed),(1<<4));
    EXPECT_EQ(get_homing_statusword_highbyte(homing_performed),(1<<4));
    EXPECT_EQ(get_homing_statusword_highbyte(homing_in_progress),0);
    EXPECT_EQ(get_homing_statusword_highbyte(homing_error_motor_standstill),(1<<5));
    EXPECT_EQ(get_homing_statusword_highbyte(homing_error_motor_turning),(1<<5));
}

TEST(LibCia402,RunTransition)
{
    run_transition(0x6);
    EXPECT_EQ(get_state(),drive_state_ready_to_switch_on);
    run_transition(0x7);
    EXPECT_EQ(get_state(),drive_state_switched_on); 
    run_transition(0x0F);
    EXPECT_EQ(get_state(),drive_state_operation_enabled); 
//    run_transition(0x82);
//    EXPECT_EQ(get_state(),drive_state_switch_on_disabled);
}

TEST(LibCia402,Homing)
{

    vcd_tracer::value<bool> neg_switch;
    vcd_tracer::value<bool> io_enable;
    vcd_tracer::value<bool> io_dir;
    vcd_tracer::value<uint8_t>cia402_state;
    vcd_tracer::value<uint8_t>highbyte;

    vcd_tracer::value<uint16_t>statusword;
    vcd_tracer::value<uint8_t>result;
    vcd_tracer::top dumper("homing");

    {
        vcd_tracer::module digital(dumper.root, "digital");
        digital.elaborate(neg_switch, "neg_switch");
        digital.elaborate(io_enable, "enable");
        digital.elaborate(io_dir, "dir");
    }
    {
        vcd_tracer::module analog(dumper.root, "statemachine");
        analog.elaborate(cia402_state, "state");
        analog.elaborate(statusword,"statusword");
        analog.elaborate(result,"result");
        analog.elaborate(highbyte,"highbyte");

    }

    std::ofstream fout("homing.vcd");
    dumper.finalize_header(fout,std::chrono::system_clock::from_time_t(0));
        
    bool mock_switch[4]={false,false,false,true};
    homing_result ex_progress_results[4]={homing_in_progress,homing_in_progress,homing_in_progress,homing_completed};
    homing_result progress_results[4];
    run_transition(0x6);
    run_transition(0x7);
    run_transition(0x0F);

    cia402_state.set(get_state());
    statusword.set(get_statusword_lowbyte(get_state()));
    EXPECT_EQ(get_state(),drive_state_operation_enabled); 
    homing_result res;
    // TODO Don't use seperate index in forrange! 
    int c=0;
    auto fi=no_fault_detected;
    for (auto i : mock_switch)
    {
        // Mock switch
        gpio_state io{
        .state=i    
        };
        auto state=get_home_switch(&io);   
        EXPECT_EQ(state,i);

        // Homing loop
        bool en= enable_disable_output_stage(&io);
        bool dir= get_direction();
        res=homing_loop(&io);
        fi=get_fault_inputs(&io);
        EXPECT_EQ(res,ex_progress_results[c]);

        // Update waveform
        neg_switch.set(state);
        io_enable.set(en);
        io_dir.set(dir);
        result.set(res);
        cia402_state.set(get_state());
        highbyte.set(get_homing_statusword_highbyte(res));
        statusword.set(get_statusword_lowbyte(get_state())|get_homing_statusword_highbyte(res)<<8);
        dumper.time_update_abs(fout, std::chrono::nanoseconds{ c });
        c++;

    }
    EXPECT_EQ(fi,fault_input_right_switch_active);
    run_transition(1<<7); //Set bit "Fault Reset"
    cia402_state.set(get_state());
    highbyte.set(get_homing_statusword_highbyte(res));
    statusword.set(get_statusword_lowbyte(get_state())|get_homing_statusword_highbyte(res)<<8);
    // EXPECT ( homing attained)
    dumper.time_update_abs(fout, std::chrono::nanoseconds{ 4 });
    
    EXPECT_EQ(get_state(),drive_state_switch_on_disabled);
    cia402_state.set(get_state());
    highbyte.set(get_homing_statusword_highbyte(res));
    statusword.set(get_statusword_lowbyte(get_state())|get_homing_statusword_highbyte(res)<<8);
    dumper.time_update_abs(fout, std::chrono::nanoseconds{ 6 });
}