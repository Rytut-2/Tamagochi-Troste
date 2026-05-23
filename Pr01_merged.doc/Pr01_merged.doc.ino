#include <WiFi.h>

// Configura los datos de tu red:
const char* ssid = "HONOR X5b";
const char* password = "1234567890";

// Pines de tus sensores (Lado Derecho)
const int humedadPin = 32;     
const int temperaturaPin = 33; 

// Tus valores de calibración reales
const int valorSeco = 4095;    
const int valorHumedo = 1250;  

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\n¡WiFi Conectado!");
  server.begin();
  
  Serial.print("Escribe esto en tu celular para jugar: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    String req = client.readStringUntil('\r');
    client.flush();
    
    // Lectura y mapeo de sensores
    int valorHumedadCrudo = analogRead(humedadPin); 
    int porcentajeHumedad = map(valorHumedadCrudo, valorSeco, valorHumedo, 0, 100);
    porcentajeHumedad = constrain(porcentajeHumedad, 0, 100);

    int valorTempCrudo = analogRead(temperaturaPin);
    float milivoltios = (valorTempCrudo / 4095.0) * 6300.0; 
    float temperaturaC = milivoltios / 10.0;

    // Endpoint API para datos AJAX
    if (req.indexOf("/leerSensores") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:application/json");
      client.println("Connection: close");
      client.println();
      client.print("{\"humedad\":" + String(porcentajeHumedad) + 
                   ",\"temp\":" + String(temperaturaC, 1) + "}");
      return;
    }

    // Servidor de la página web principal
    if (req.indexOf("GET / ") != -1 || req.indexOf("GET /index.html") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html; charset=utf-8");
      client.println("Connection: close");
      client.println();
      
      client.println("<!DOCTYPE html><html>");
      client.println("<head><meta name='viewport' content='width=device-width, initial-scale=1'>");
      client.println("<title>Eco-Tamagotchi</title>");
      
      // Estilos CSS para el fondo dinámico del jardín y el cascarón
      client.println("<style>");
      // Fondo general interactivo: Paisaje natural pixelado con CSS puro
      client.println("body { font-family: 'Courier New', monospace; background: linear-gradient(to bottom, #70a1ff, #f1f2f6); display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; user-select: none; overflow: hidden; position: relative; }");
      client.println(".grass { position: absolute; bottom: 0; left: 0; width: 100%; height: 25vh; background-color: #2ed573; border-top: 6px dashed #26af56; z-index: 1; }");
      
      // Elementos del paisaje: Árboles
      client.println(".tree { position: absolute; bottom: 23vh; background-color: #10ac84; border-radius: 50% 50% 0 0; z-index: 2; border: 4px solid #222; }");
      client.println(".tree::after { content: ''; position: absolute; bottom: -30px; left: 40%; width: 20%; height: 30px; background-color: #57370d; border: 3px solid #222; }");
      client.println(".tree-left { left: 4%; width: 90px; height: 160px; }");
      client.println(".tree-right { right: 5%; width: 110px; height: 190px; }");
      
      // Elementos del paisaje: Macetas y Plantas en el suelo
      client.println(".pot-garden { position: absolute; bottom: 8vh; z-index: 3; display: flex; gap: 15px; }");
      client.println(".pot-left { left: 15%; } .pot-right { right: 15%; }");
      client.println(".mini-pot { background: #e67e22; width: 35px; height: 30px; border-radius: 0 0 5px 5px; border: 3px solid #222; position: relative; }");
      client.println(".mini-pot::before { content: '🌿'; position: absolute; top: -24px; left: 2px; font-size: 22px; }");
      client.println(".mini-flower::before { content: '🌸'; position: absolute; top: -24px; left: 2px; font-size: 20px; }");
      
      // Nubes decorativas
      client.println(".cloud { background: white; width: 120px; height: 40px; border-radius: 20px; position: absolute; top: 10%; opacity: 0.8; }");
      client.println(".cloud-1 { left: 10%; transform: scale(0.8); } .cloud-2 { right: 15%; top: 15%; }");
      
      // Estilos del Cascarón Tamagotchi (Z-index superior para estar al frente)
      client.println(".tamagotchi-case { width: 320px; height: 420px; border-radius: 50% 50% 45% 45%; padding: 30px; display: flex; flex-direction: column; align-items: center; justify-content: flex-start; border: 4px solid #333; position: relative; transition: all 0.3s ease; z-index: 10; box-shadow: 0px 15px 35px rgba(0,0,0,0.4); }");
      client.println(".bg-coral { background: #ff6b6b; box-shadow: inset 0px -10px 0px #c0392b; }");
      client.println(".bg-blue { background: #3498db; box-shadow: inset 0px -10px 0px #2980b9; }");
      client.println(".bg-yellow { background: #f1c40f; box-shadow: inset 0px -10px 0px #f39c12; }");
      client.println(".bg-green { background: #2ecc71; box-shadow: inset 0px -10px 0px #27ae60; }");
      
      client.println(".brand { color: white; font-weight: bold; font-size: 14px; margin-bottom: 20px; letter-spacing: 2px; text-shadow: 1px 1px 0px #333; }");
      client.println(".screen { background-color: #a3cb38; width: 240px; height: 220px; border-radius: 15px; border: 4px solid #333; box-shadow: inset 0px 0px 15px rgba(0,0,0,0.3); display: flex; flex-direction: column; justify-content: space-between; padding: 15px; box-sizing: border-box; color: #2c3e50; }");
      client.println(".stats { display: flex; justify-content: space-between; font-size: 13px; font-weight: bold; border-bottom: 2px dashed #2c3e50; padding-bottom: 5px; }");
      client.println(".pet-avatar { font-size: 60px; margin: 5px 0; text-align: center; animation: bounce 1s infinite alternate; height: 75px; display: flex; align-items: center; justify-content: center; }");
      client.println(".status-text { text-align: center; font-weight: bold; font-size: 12px; min-height: 45px; line-height: 1.3; display: flex; align-items: center; justify-content: center; }");
      client.println(".buttons-container { display: flex; justify-content: space-around; width: 85%; margin-top: 25px; }");
      client.println(".btn-wrapper { display: flex; flex-direction: column; align-items: center; color: white; font-size: 9px; font-weight: bold; text-shadow: 1px 1px 0px #333; }");
      client.println(".btn { width: 38px; height: 38px; background: #ecf0f1; border-radius: 50%; border: 3px solid #333; box-shadow: 0px 4px 0px #7f8c8d; cursor: pointer; margin-bottom: 4px; }");
      client.println(".btn:active { transform: translateY(4px); box-shadow: none; }");
      client.println("@keyframes bounce { from { transform: translateY(0); } to { transform: translateY(-6px); } }");
      client.println("</style>");
      
      // Lógica de JavaScript para botones, consejos y colores
      client.println("<script>");
      client.println("var modoConsejo = false;");
      client.println("var colores = ['bg-coral', 'bg-blue', 'bg-yellow', 'bg-green'];");
      client.println("var colorActual = 0;");
      
      client.println("var consejos = [");
      client.println("  'No riegues al mediodía, el agua se evapora muy rápido.',");
      client.println("  'Asegúrate de que la maceta tenga buen drenaje o las raíces se pudrirán.',");
      client.println("  'Limpia el polvo de las hojas con un paño húmedo para que respiren.',");
      client.println("  'Agrupa tus plantas para crear un microclima con mejor humedad.',");
      client.println("  'Si las puntas están marrones, les falta humedad ambiental.',");
      client.println("  'El agua de lluvia es excelente para regar tus plantas.',");
      client.println("  'Gira la maceta 90 grados cada semana para que crezca recta.',");
      client.println("  'Es mejor quedarse corto con el riego que pasarse.',");
      client.println("  'Investiga si tu planta prefiere luz directa o sombra.',");
      client.println("  'Fertiliza solo durante la primavera y el verano.'");
      client.println("];");
      
      client.println("function actualizarTamagotchi() {");
      client.println("  if (modoConsejo) return;");
      
      client.println("  var xhttp = new XMLHttpRequest();");
      client.println("  xhttp.onreadystatechange = function() {");
      client.println("    if (this.readyState == 4 && this.status == 200) {");
      client.println("      var datos = JSON.parse(this.responseText);");
      client.println("      document.getElementById('humi').innerHTML = '💧' + datos.humedad + '%';");
      client.println("      document.getElementById('temp').innerHTML = '🌡️' + datos.temp + '°C';");
      
      client.println("      var avatar = document.getElementById('pet');");
      client.println("      var texto = document.getElementById('status');");
      
      // Lógica de emociones
      client.println("      if (datos.humedad <= 15) {");
      client.println("        avatar.innerHTML = '💀';");
      client.println("        texto.innerHTML = 'ESTADO: CRÍTICO<br>¡Tengo sed extrema! Ponme agua.';");
      client.println("      } else if (datos.humedad > 15 && datos.humedad <= 40) {");
      client.println("        avatar.innerHTML = '🥺';");
      client.println("        texto.innerHTML = 'ESTADO: TRISTE<br>Suelo seco... ¿Me das agua?';");
      client.println("      } else if (datos.humedad > 40 && datos.humedad <= 70) {");
      client.println("        avatar.innerHTML = '⭐‿⭐';");
      client.println("        texto.innerHTML = 'ESTADO: FELIZ<br>¡Humedad perfecta! Todo bien.';");
      client.println("      } else if (datos.humedad > 70 && datos.humedad <= 85) {");
      client.println("        avatar.innerHTML = '🥴';");
      client.println("        texto.innerHTML = 'ESTADO: SATURADA<br>Mucha agua por aquí abajo...';");
      client.println("      } else {");
      client.println("        avatar.innerHTML = '🤢';");
      client.println("        texto.innerHTML = 'ESTADO: AHOGADA<br>¡Basta de agua! Me pudro.';");
      client.println("      }");
      
      client.println("      if (datos.temp > 38) {");
      client.println("        avatar.innerHTML = '🥵';");
      client.println("        texto.innerHTML = '¡ALERTA CALOR!<br>¡Me aso! Quítame del sol.';");
      client.println("      }");
      client.println("    }");
      client.println("  };");
      client.println("  xhttp.open('GET', '/leerSensores', true);");
      client.println("  xhttp.send();");
      client.println("}");
      
      client.println("function mostrarConsejo() {");
      client.println("  modoConsejo = true;");
      client.println("  var indiceRandom = Math.floor(Math.random() * consejos.length);");
      client.println("  document.getElementById('humi').innerHTML = '💡';");
      client.println("  document.getElementById('temp').innerHTML = 'CONSEJO';");
      client.println("  document.getElementById('pet').innerHTML = '📖';");
      client.println("  document.getElementById('status').innerHTML = consejos[indiceRandom];");
      client.println("}");
      
      client.println("function regresarMenu() {");
      client.println("  modoConsejo = false;");
      client.println("  document.getElementById('status').innerHTML = 'Regresando...';");
      client.println("  actualizarTamagotchi();");
      client.println("}");
      
      client.println("function cambiarColor() {");
      client.println("  var carcasa = document.getElementById('carcasa');");
      client.println("  carcasa.classList.remove(colores[colorActual]);");
      client.println("  colorActual = (colorActual + 1) % colores.length;");
      client.println("  carcasa.classList.add(colores[colorActual]);");
      client.println("}");
      
      client.println("setInterval(actualizarTamagotchi, 1500);");
      client.println("window.onload = actualizarTamagotchi;");
      client.println("</script>");
      client.println("</head>");
      
      // Estructura HTML con el ecosistema de fondo y el dispositivo al frente
      client.println("<body>");
      
      // CAPAS DEL FONDO DE JARDÍN RETRO
      client.println("  <div class='cloud cloud-1'></div>");
      client.println("  <div class='cloud cloud-2'></div>");
      client.println("  <div class='tree tree-left'></div>");
      client.println("  <div class='tree tree-right'></div>");
      client.println("  <div class='grass'></div>");
      
      // Macetas agrupadas a la izquierda y derecha en el suelo
      client.println("  <div class='pot-garden pot-left'>");
      client.println("    <div class='mini-pot'></div>");
      client.println("    <div class='mini-pot mini-flower'></div>");
      client.println("  </div>");
      client.println("  <div class='pot-garden pot-right'>");
      client.println("    <div class='mini-pot mini-flower'></div>");
      client.println("    <div class='mini-pot'></div>");
      client.println("  </div>");
      
      // CONSOLA CENTRAL DEL TAMAGOTCHI
      client.println("  <div id='carcasa' class='tamagotchi-case bg-coral'>");
      client.println("    <div class='brand'>TAMAPOTCHI v1.0</div>");
      client.println("    <div class='screen'>");
      client.println("      <div class='stats'>");
      client.println("        <span id='humi'>💧--%</span>");
      client.println("        <span id='temp'>🌡️--°C</span>");
      client.println("      </div>");
      client.println("      <div class='pet-avatar' id='pet'>😐</div>");
      client.println("      <div class='status-text' id='status'>Despertando...</div>");
      client.println("    </div>");
      
      // Botonera Interactiva
      client.println("    <div class='buttons-container'>");
      client.println("      <div class='btn-wrapper'>");
      client.println("        <div class='btn' onclick='mostrarConsejo()'></div>INFO");
      client.println("      </div>");
      client.println("      <div class='btn-wrapper' style='margin-top: 10px;'>");
      client.println("        <div class='btn' onclick='regresarMenu()'></div>MENU");
      client.println("      </div>");
      client.println("      <div class='btn-wrapper'>");
      client.println("        <div class='btn' onclick='cambiarColor()'></div>COLOR");
      client.println("      </div>");
      client.println("    </div>");
      client.println("  </div>");
      
      client.println("</body></html>");
    }
  }
}