import pygame
import serial
import time
import random
pygame.init()

ser = serial.Serial('COM3', 9600)
time.sleep(2)
screen = pygame.display.set_mode((1024, 1024))
pygame.display.set_caption('Joystick game')
font = pygame.font.SysFont(None, 55)

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
GREEN = (0, 255, 0)
RED = (255, 0, 0)

player_size = 20
player_pos = [512, 512]

obstacle_size = 20
obstacle_pos = [random.randint(0, 1024-obstacle_size), 0]
obstacle_speed = 10

point = 0
collision_time = 0
start_time = pygame.time.get_ticks()

running = True
clock = pygame.time.Clock()

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    if ser.in_waiting > 0:
        data = ser.readline().decode('ISO-8859-1').strip()
        if data:
            try:
                x, y = map(int, data.split(','))
                player_pos = [x - player_size // 2, y - player_size // 2]
            except ValueError:
                continue
    print(f"{x}, {y}")
    obstacle_pos[1] += obstacle_speed
    if obstacle_pos[1] > 1024:
        obstacle_pos = [random.randint(0, 1024-obstacle_size), 0]

    current_time = pygame.time.get_ticks()
    if (obstacle_pos[0] < player_pos[0] < obstacle_pos[0] + obstacle_size or 
        obstacle_pos[0] < player_pos[0] + player_size < obstacle_pos[0] + obstacle_size) and (
        obstacle_pos[1] < player_pos[1] < obstacle_pos[1] + obstacle_size or 
        obstacle_pos[1] < player_pos[1] + player_size < obstacle_pos[1] + obstacle_size):
        if current_time - collision_time > 1000:
            point += 1
            collision_time = current_time

    if point == 10:
        elapsed_time = (pygame.time.get_ticks() - start_time) // 1000
        running = False

    screen.fill(BLACK)
    pygame.draw.rect(screen, GREEN, (player_pos[0], player_pos[1], player_size, player_size))  # キャラクターを描画
    pygame.draw.rect(screen, RED, (obstacle_pos[0], obstacle_pos[1], obstacle_size, obstacle_size))  # 障害物を描画

    score_text = font.render(f"Points: {point}", True, WHITE)
    screen.blit(score_text, (10, 10))

    elapsed_time_text = font.render(f"Time: {(pygame.time.get_ticks() - start_time) // 1000} s", True, WHITE)
    screen.blit(elapsed_time_text, (10, 70))

    pygame.display.flip()
    clock.tick(60)
ser.close()
pygame.quit()

print(f"10ポイントに到達するまでの秒数: {elapsed_time}秒")
