# TriploS
O TriploS é uma ferramenta de detecção e avaliação para redes sem fio IEEE 802.11. Esta ferramenta permite coletar informações como nível de sinal, qualidade, MAC Addresse e SSID, além de fornecer métricas de avaliação para seleção do melhor canal disponível. 

Instalação

Extraia a ferramenta utilizando a ferramenta unzip a partir do seguinte comando:

$unzip triplos.zip

Entre no diretório triplos

$cd triplos/

Execute o script install.sh como sudo

$sudo ./install.sh

Para executar o TriploS é necessário definir a os parâmetros -n (numérios de amostras) e -t (intervalo de tempo entre as amostras). 

$sudo ./triplos -n 10 -t 1

Para salvar as análises das redes sem fio em arquivo, o parâmetro -f deverá sem utilizado, por padrão os arquivos serão salvos no diretório "~/triplos/netdata", cada rede sem-fio gerará um arquivo com o nome de seu MAC Address.


Para geração dos gráficos as partir dos dados coletados deverá ser utilizado o parâmetro -c.

Os gráficos são armazenados no diretório "~/triplos/charts", caso seja necessário alguma alteração no cabeçalho ou configurações dos gráficos, os mesmos poderão ser editados a partir dos arquivos .gnu correspondentes, disponíveis no diretório "~/triplos/gnu".

Para mais informações acesse: http://etss.ufam.edu.br/index.php/ferramentas/66-triplos ou nos contate (alex.monteirO@icomp.ufam.edu.br)
