import flet as ft
import serial.tools.list_ports
import speech_recognition as sr

def main(page: ft.Page):
  page.title = "Dr_stone Petrification Ray"
  page.window.width = 300
  page.window.height = 400
  page.window.min_width = 300
  page.window.min_height = 400

  def update_ports(e):
    ports = serial.tools.list_ports.comports()
    portsList = [str(port.device) for port in ports]  # COMポート名だけを取得
    ports_dropdown.options = [ft.dropdown.Option(port) for port in portsList]
    page.update()

  def connect_serial(e):
    com_port = ports_dropdown.value
    serialInst.port = com_port
    serialInst.baudrate = 9600
    try:
      serialInst.open()
      status_label.value = f"{com_port} に接続しました"
    except Exception as ex:
      status_label.value = f"接続に失敗しました: {ex}"
    page.update()
  
  def send_command(command):
    if serialInst.is_open:
      serialInst.write(command.encode('utf-8'))
      if command == 'exit':
        serialInst.close()
        status_label.value = "切断されました"
      else:
        status_label.value = f"コマンドを送信しました: {command}"
    else:
      status_label.value = "シリアルポートが接続されていません"
    page.update()

  def start_listening(e):
    r = sr.Recognizer()
    mic = sr.Microphone()

    with mic as source:
      r.adjust_for_ambient_noise(source)
      audio = r.listen(source)

    try:
      text = r.recognize_google(audio, language='ja-JP')
      output.value = text
      page.update()

      send_command(text)

      if "ストップ" in text:
        output.value = "プログラムを終了します"
        page.update()
        return

    except sr.UnknownValueError:
      output.value = "認識できなかった"
      page.update()
    except sr.RequestError as e:
      output.value = f"Could not request results from Google Speech Recognition service; {e}"
      page.update()
  
  serialInst = serial.Serial()
  ports_dropdown = ft.Dropdown()
  refresh_button = ft.Button(text="ポート更新", on_click=update_ports)
  connect_button = ft.Button(text="接続", on_click=connect_serial)
  command_input = ft.TextField(label="Arduinoコマンド (～メートル ～秒)")
  send_button = ft.Button(text="コマンド送信", on_click=lambda e: send_command(command_input.value))
  status_label = ft.Text()
  output = ft.Text(value="なにか話してください ...", size=20)
  start_button = ft.ElevatedButton(text="Start Listening", on_click=start_listening)
  
  page.add(
    ports_dropdown,
    refresh_button,
    connect_button,
    command_input,
    send_button,
    status_label,
    output,
    start_button
  )

ft.app(target=main)