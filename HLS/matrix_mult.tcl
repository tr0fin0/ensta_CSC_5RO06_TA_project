############################################################
## Loop through different solutions and execute synthesis
## C/RTL cosimulation, and export RTL to VHDL with a custom name
############################################################

# Open the project
open_project matrix_multiplication

# Define the top-level function (this will change dynamically)
set top_function ""

# Define solutions in a list
set full_exec 0
set sizes {8 16 32 64 128}
set solutions {0 1 2 3}

# Iterate over each solution
foreach solution ${solutions} {
    foreach size ${sizes} {
        # Set the top function based on the solution (matrix_mult_0, matrix_mult_1, ...)
        set top_function "matrix_mult_${solution}"
        puts "top_function: ${top_function} with SIZE=${size}."

        # Set top-level function in HLS
        set_top $top_function

        # Add source files and define which solution to compile using -D flag
        add_files matrix_mult.h -cflags "-Dsolution${solution} -DSIZE=${size}"
        add_files matrix_mult.cpp -cflags "-Dsolution${solution} -DSIZE=${size}"

        # Add testbench files (adjust paths if needed)
        add_files -tb matrix_mult_tb.h -cflags "-Wno-unknown-pragmas"
        add_files -tb matrix_mult_tb.cpp -cflags "-Wno-unknown-pragmas -Dsolution${solution} -DSIZE=${size}"

        # Open solution for the specific solution solution
        open_solution "solution_${solution}_${size}"

        # Set the part
        set_part {xc7z020clg484-1} -tool vivado

        # Define clock period and uncertainty
        create_clock -period 10 -name default
        set_clock_uncertainty 1.25

        if {$full_exec} {
            # Run C Simulation (optional)
            csim_design
        }

        # Run Synthesis
        csynth_design

        if {$full_exec} {
            # Run C/RTL Cosimulation
            cosim_design -rtl vhdl

            # Export RTL with custom name based on solution
            set ip_name "${top_function}_${size}_IP"
            export_design -format ip_catalog -rtl vhdl -ipname ${ip_name}
        }

        # Close the solution to prepare for the next iteration
        close_solution
    }
}

# Close the project when done
close_project
