- [ ] MPTT
  - [ ] Probar las distantas entradas
    - [ ] Probar con la fuente
    - [ ] Probar con paneles
  - [ ] calcular la perdida de potencia de cada fuente
- [ ] BMS
  - [ ] hacer andar el bms
  - [ ] hacer que cierre el mosfet
  - [x] leer datos
  - [ ] Configurar el BMS
    - [ ] limites de voltage UV Y OV
    - [ ] limites de corrinte (over current in discharge and OCIC)
    - [ ] short circuit detection
    - [ ] roteccion por temperatura alta o baja en carga y descarga
- [ ] PDU

  - [x] Probar las fuentes y los ADCS
  - [ ] Calcular la eficiencia de cada fuente

- [ ] I2C con las demas placas

  - [ ] codificar comandos y hacer manual de uso



- [ ] Preguntar a enrique


la corriente tiene que ser cte para medir? o podemos usar una resistencia

como le mostramos las mediciones si hacemos un video o hacemos un meet un dai para que ellos lo vean

"Descargar la batería a corriente constante
de 0,5C midiendo  la tensión de salida 
hasta el corte del sistema. Registrar la 
tensión en el momento del corte."			
			
como seria la medicion esta? si la corriente es de 2800 mAh descargamos a 1400 mA y cuando el sistema porte por UV medimos con un tester para comprobar que efectivamente corto en la tensino indicada?


"Partir del estado de descarga total
Cargar la batería a 0,2C medir tensión
Cargar la batería a 0,5C medir tensión
Cargar la  batería a 1C medir tensión
La batería debe ser cargada a corriente
constante con limitación de la tensión de
tal modo que el final de carga se produzca
a tensión constante. GRAFICAR"			

la curva de carga y la curva de descarga no se aclara por cuanto tiempo hay que graficar las curvas?


Medidor de capacidad			
"Durante la medición de las curvas de
carga y descarga verificar la lectura que 
suministra el contador de carga y hacer
una tabla de comparación entre los valores
medidos y los reportados por el sistema."			
						
						
						
No entendemos a que te referis a los valores reportados por el sistema, nosotros solo tenemos 1 medicon lo que nos da el coulomb counter, el bms no reporta automaticamente la carga, es por ello que hacemos uso de un tiemer.











- [ ] hacer informe final con los datos obtenids

-especificaciones precias al des;o que se esperaba de cada parte

psubsistemas del eps

describo cada subsistema

requerimientos de cada sussitema

estandads de fabricacion y comparacion con otros EPS

requerimientos aeroespaciales

- cosas que tendriamos que haver hacho versus lo que hicimos

- posibles dise;os para cada parte

dise;i a con componentes nacionales y posibilidades

- diseño b comprando componenes en el experior peor con limitaciones por el tamaño de los componenetes

- diceño c mandnado a fabricar a china con todos los componentes y funciones

eleccion del microcontrolador 
-stm
-esp
-arduino

pros y contras
eleccion final

inconvenientes con el stm32 

problemas y soluciones con la plca

#### bms 
- bq76905

- bq29330
problemas y falta de gaus gauge

describriemiento del ina219 y su bus i2c
