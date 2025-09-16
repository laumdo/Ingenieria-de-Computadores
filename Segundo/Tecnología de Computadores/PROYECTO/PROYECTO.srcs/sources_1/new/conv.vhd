library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity conv is
  port (x       : in  std_logic_vector (1 downto 0);
        display : out std_logic_vector (6 downto 0));
end conv;

architecture rtl of conv is

begin

  with x select
    display <=  not "0111111" when "00",
    not "0000110"            when "01",
    not "1011011"            when "10",
    not "0000000"            when others;

end rtl;