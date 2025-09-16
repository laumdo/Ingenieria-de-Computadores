library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity displays is
    Port ( 
        rst : in STD_LOGIC;
        clk : in STD_LOGIC;       
        digito_0 : in  STD_LOGIC_VECTOR (1 downto 0);
        digito_1 : in  STD_LOGIC_VECTOR (1 downto 0);
        digito_2 : in  STD_LOGIC_VECTOR (1 downto 0);
        digito_3 : in  STD_LOGIC_VECTOR (1 downto 0);
        aleatorio: in STD_LOGIC_VECTOR (1 downto 0);
        display : out  STD_LOGIC_VECTOR (6 downto 0);
        display_enable : out  STD_LOGIC_VECTOR (3 downto 0)
     );
end displays;

architecture Behavioral of displays is

    component conv
        Port ( x : in  STD_LOGIC_VECTOR (1 downto 0);
               display : out  STD_LOGIC_VECTOR (6 downto 0));
    end component;

    signal display_0 : STD_LOGIC_VECTOR (6 downto 0);
    signal display_1 : STD_LOGIC_VECTOR (6 downto 0);
    signal display_2 : STD_LOGIC_VECTOR (6 downto 0);
    signal display_3 : STD_LOGIC_VECTOR (6 downto 0);
    

begin

    conv_7seg_digito_0 : conv port map(x => digito_0, display => display_0);
    conv_7seg_digito_1 : conv port map(x => digito_1, display => display_1);
    conv_7seg_digito_2 : conv port map(x => digito_2, display => display_2);
    conv_7seg_digito_3 : conv port map(x => digito_3, display => display_3);

    display <=  display_0 when (aleatorio(1 downto 0) = "00") else
                display_1 when (aleatorio(1 downto 0) = "01") else
                display_2 when (aleatorio(1 downto 0) = "10") else
                display_3  when (aleatorio(1 downto 0) = "11");

    display_enable <=   "1100" when (aleatorio(1 downto 0) = "00") else
                        "1100" when (aleatorio(1 downto 0) = "01") else
                        "1100" when (aleatorio(1 downto 0) = "10") else
                        "0100";

end Behavioral;