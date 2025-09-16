library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity sintesis is
    Port ( 
       rst: IN std_logic;
       clk: IN std_logic;
       boton: IN std_logic;
       opcion: IN std_logic_vector(1 downto 0);
       leds: OUT std_logic_vector(15 downto 0);       
       salida_display: OUT std_logic_vector(6 downto 0); --display
       salida_display_enable: OUT std_logic_vector(3 downto 0) --display
   );
end sintesis;

architecture Behavioral of sintesis is
    component juego is 
     Port ( 
       rst: IN std_logic;
       clk: IN std_logic;
       boton: IN std_logic;
       opcion: IN std_logic_vector(1 downto 0);
       leds: OUT std_logic_vector(15 downto 0);
       maquina: OUT std_logic_vector(1 downto 0);
       salida_display: OUT std_logic_vector(6 downto 0); --display
       salida_display_enable: OUT std_logic_vector(3 downto 0) --display
   );
   end component;
   component debouncer is
     PORT (
        rst: IN std_logic;
        clk: IN std_logic;
        x: IN std_logic;
        xDeb: OUT std_logic;
        xDebFallingEdge: OUT std_logic;
        xDebRisingEdge: OUT std_logic
      );
  end component;
  component conv is
    port (x       : in  std_logic_vector (1 downto 0);
        display : out std_logic_vector (6 downto 0));
  end component;
  SIGNAL maquina : std_logic_vector (1 DOWNTO 0);
  SIGNAL salidaDebouncer, srst : std_logic;
  
begin
    srst <= not rst;
	debouncerInstance: debouncer PORT MAP (srst, clk, boton, open, open, salidaDebouncer);
    juegoInstance : juego PORT MAP (rst, clk, salidaDebouncer, opcion,leds, maquina );
	conv_7segInstance: conv PORT MAP (maquina, salida_display);
	salida_display_enable <= "1110";

end Behavioral;
