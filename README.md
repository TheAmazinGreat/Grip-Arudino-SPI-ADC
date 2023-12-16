# Grip-Arudino-SPI-ADC
Arduino and shift register combinations as an SPI slave to be used with flight sim grips for buttons and axes. Designed to use Adafruit Metro Mini connected to VPForce FFB mainboard. 

# DISCLAIMER
All code should be used at your own risk. I do not accept responsibilty for damages or loss due to implementation of this code. I am not a software engineer, there are more than likely ways that this code can be improved. Please feel free to make suggestions for improvements but as long as things are working as intended I may reserve the right to keep things as is. 

# Arduino with Shift Register
Use the files in "Arduino+Shift-Register" if you have connected a shift register in series with your Arduino to expand the inputs. This was orignally designed for the VPForce Shift Register board but should work with other shift registers that use CD4021B shift registers or SN74HC165 shift registers with an inverted latch signal. Please review the connection diagrams of those chips prior to connection to verify proper circuit configurations. 

# Arduino Only
Use the files in "Arduino-Only" if you do not have a shift register and are only using the input pins on your Arduino for buttons. 
