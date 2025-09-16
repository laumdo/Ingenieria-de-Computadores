library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity generadorAleatorio is
  Port ( 
  clk: in std_logic;
  rst: in std_logic;
  enable: in std_logic;
  numero: out std_logic_vector(1 downto 0));
end generadorAleatorio;

architecture Behavioral of generadorAleatorio is

signal aleatorio: std_logic_vector(1 downto 0);
begin

    process(clk,rst)
    begin
        if rst='1' then
            aleatorio<=(others=> '0');  -- Reiniciar la semilla en caso de reset
        elsif rising_edge(clk) then
            if enable='1' then
             if aleatorio="11" then
                aleatorio <="00";
             else 
                    aleatorio <=aleatorio+1;
             end if;
            end if;
        end if;
        numero<= aleatorio;
    end process;
end Behavioral;
