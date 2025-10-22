<!DOCTYPE html>
<html lang="ru">
<head>
    <title>Конструктор ПК</title>
    <script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.0/jquery.min.js"></script>
    <script type="text/javascript" src="script.js"></script>
    <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
	<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
	<!----------------------------------------------->
	<p> <b>Материнская плата: </b></p>
    <form>
		<p> <label> <span id="value_motherboard"> </span> </label> </p>
        <label> <input type="text" id="search_motherboard" placeholder="Начните вводить..." /> </label>
    </form>	
    <div id="display_motherboard"></div>
	<!----------------------------------------------->
	<p><b>Процессор (CPU): </b></p>
    <form>
		<p> <label> <span id="value_cpu"> </span> </label> </p>
        <label> <input type="text" id="search_cpu" placeholder="Начните вводить..." /> </label>
		<div id="display_cpu"></div>
    </form>
	<!----------------------------------------------->    
	<p> <b>Кулер процессора (CPU): </b></p>
    <form>
        <p> <label> <span id="value_cpu_fan"> </span> </label> </p>
		<label> <input type="text" id="search_cpu_fan" placeholder="Начните вводить..." /> </label>
		<div id="display_cpu_fan"></div>
    </form>
	<!----------------------------------------------->
	<p> <b>Оперативная память (RAM): </b> </p>
    <form>
		<p> <div id="value_ram"></div> </p>
		<p> <div id="test_ram"></div> </p>
        <label> <input type="text" id="search_ram" placeholder="Начните вводить..." /> </label>	
		<div id="display_ram"></div>
    </form>
	<!----------------------------------------------->
	<p> <b>Накопители информации: </b></p>
	<form>  
        <label>Количество жестких дисков и SSD накопителей: </label>
		<div>
			<input type="number" id="sata_count" min="0" max="50" value="0">
			<div>
				<label>Количество M.2 накопителей: </label>  
				<div>
					<input type="number" id="m2_count" min="0" max="50" value="0"> 
				</div>
			</div>
		</div>
    </form> 
	<!----------------------------------------------->
	<p> <b>Видеокарта (GPU) </b></p>
    <form>
		<p> <div id="value_gpu"></div> </p>
		<p> <div id="test_gpu"></div> </p>
        <label> <input type="text" id="search_gpu" placeholder="Начните вводить..." /> </label>	
		<div id="display_gpu"></div>	
    </form>
	<!----------------------------------------------->
	<p> <b>Блок питания (PSU):</b></p>
        <label>
			<p> <div id="value_psu"></div> </p>
            <input type="text" id="search_psu" placeholder="Начните вводить..." />
			<div id="display_psu"></div>	
        </label>	
    </form>
	<!----------------------------------------------->
	<p><b>Корпус: </b></p>
    <form>
        <label>
			<p> <div id="value_pc_case"></div> </p>
            <input type="text" id="search_pc_case" placeholder="Начните вводить..." />
			<div id="display_pc_case"></div>
        </label>	
    </form>
	<!----------------------------------------------->
	<p></p>
	<form>
		<button id="check" disabled="disabled">
			 Проверить совместимость
		</button>
	</form>

</body>
</html>