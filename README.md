# ReguladorPI_PT100_VariadorSchneider
 Mediante una fuente de alimentación se suministra tensión continua a una resistencia de potencia que, debido a la disipación de calor, tenderá a aumentar su temperatura, que se medirá con un sensor PT100. Este sensor está conectado a un módulo de conversión de señal, de forma que la temperatura se transforma en una señal 0-20 mA que se lleva a una entrada analógica de un variador de velocidad Schneider Altivar. Mediante una hélice accionada con un motor trifásico que se maneja desde el variador, se crea un flujo de aire variable en la resistencia y así se regula su temperatura. Se implanta un regulador PI, de forma que se pueda configurar sus parámetros Kp y Ti y el período de muestreo y sea posible fijar la actuación mínima y máxima. Se crea una aplicación para el control de temperatura de este sistema, pudiendo fijar con el teclado la consigna y visualizando en pantalla su valor y también la temperatura medida y la actuación aplicada.