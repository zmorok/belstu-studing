import pygame
import random
import sys
import os

pygame.init()

WIDTH, HEIGHT = 1000, 600
FPS = 144
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Что-то/лов оффлайн 3000")
clock = pygame.time.Clock()

FONT = pygame.font.SysFont(None, 36)

ASSETS_DIR = "assets"
ENEMY_DIR = os.path.join(ASSETS_DIR, "enemies")

def load_sound(filename: str):
    path = os.path.join(ASSETS_DIR, filename)
    try:
        return pygame.mixer.Sound(path)
    except:
        print(f"Ошибка загрузки звука '{path}'. Запуск без звука.")
        return None


def load_image(filename, size):
    path = os.path.join(ASSETS_DIR, filename)
    try:
        img = pygame.image.load(path).convert_alpha()
        if size:
            img = pygame.transform.smoothscale(img, size)
        return img
    except pygame.error:
        print(f"Ошибка загрузки фото '{path}'. Дефолт картинка.")
        return None


collect_sfx = load_sound("coin.mp3")
hit_sfx = load_sound("hit.wav")

# изображения врагов
ENEMY_IMAGES = []
if os.path.isdir(ENEMY_DIR):
    for fn in os.listdir(ENEMY_DIR):
        if fn.lower().endswith((".png", ".jpg", ".jpeg")):
            img = load_image(os.path.join("enemies", fn), (100, 100))
            if img:
                ENEMY_IMAGES.append(img)

# сложности
def get_enemy_parameters(score):
    if score < 20:
        return 3, 6, 1500
    elif score < 50:
        return 4, 7, 1200
    elif score < 100:
        return 5, 8, 900
    else:
        return 6, 9, 700

# спрайты
class Player(pygame.sprite.Sprite):
    def __init__(self):
        super().__init__()
        img = load_image("player.png", (50, 50))
        if img:
            self.image = img
        else:
            self.image = pygame.Surface((50, 50))
            self.image.fill((0, 200, 0))
        self.rect = self.image.get_rect(center=(WIDTH // 2, HEIGHT - 60))
        self.speed = 6
        self.health = 3

    def update(self, pressed):
        if pressed[pygame.K_UP] or pressed[pygame.K_w]:
            self.rect.y -= self.speed
        if pressed[pygame.K_LEFT] or pressed[pygame.K_a]:
            self.rect.x -= self.speed
        if pressed[pygame.K_DOWN] or pressed[pygame.K_s]:
            self.rect.y += self.speed   
        if pressed[pygame.K_RIGHT] or pressed[pygame.K_d]:
            self.rect.x += self.speed
        
        self.rect.clamp_ip(screen.get_rect())


class Coin(pygame.sprite.Sprite):
    def __init__(self):
        super().__init__()
        self.image = pygame.Surface((30, 30), pygame.SRCALPHA)
        pygame.draw.circle(self.image, (255, 215, 0), (15, 15), 15)
        self.rect = self.image.get_rect(center=(random.randint(15, WIDTH - 15), -20))
        self.speed = random.randint(2, 5)

    def update(self):
        self.rect.y += self.speed
        if self.rect.top > HEIGHT:
            self.kill()


class Enemy(pygame.sprite.Sprite):
    def __init__(self, speed_range):
        super().__init__()
        if ENEMY_IMAGES:
            self.image = random.choice(ENEMY_IMAGES).copy()
        else:
            self.image = pygame.Surface((100, 100))
            self.image.fill((200, 50, 50))
        self.rect = self.image.get_rect(center=(random.randint(50, WIDTH - 50), -20))
        self.speed = random.randint(*speed_range)

    def update(self):
        self.rect.y += self.speed
        if self.rect.top > HEIGHT:
            self.kill()


def game_over_screen(score) -> bool:
    button_w, button_h = 200, 60
    play_again_rect = pygame.Rect((WIDTH // 2 - button_w // 2, HEIGHT // 2), (button_w, button_h))
    quit_rect = pygame.Rect((WIDTH // 2 - button_w // 2, HEIGHT // 2 + 80), (button_w, button_h))

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                if play_again_rect.collidepoint(event.pos):
                    return True
                if quit_rect.collidepoint(event.pos):
                    return False

        screen.fill((30, 30, 45))

        title = FONT.render("GAME OVER", True, (255, 255, 255))
        score_text = FONT.render(f"Ваш рекорд: {score}", True, (255, 255, 255))
        play_text = FONT.render("Играть снова?", True, (0, 0, 0))
        quit_text = FONT.render("Выйти", True, (0, 0, 0))

        pygame.draw.rect(screen, (100, 200, 100), play_again_rect, border_radius=10)
        pygame.draw.rect(screen, (200, 100, 100), quit_rect, border_radius=10)

        screen.blit(title, title.get_rect(center=(WIDTH // 2, HEIGHT // 2 - 100)))
        screen.blit(score_text, score_text.get_rect(center=(WIDTH // 2, HEIGHT // 2 - 50)))
        screen.blit(play_text, play_text.get_rect(center=play_again_rect.center))
        screen.blit(quit_text, quit_text.get_rect(center=quit_rect.center))

        pygame.display.flip()
        clock.tick(FPS)


def run_game() -> bool:
    all_sprites = pygame.sprite.Group()
    coins = pygame.sprite.Group()
    enemies = pygame.sprite.Group()

    player = Player()
    all_sprites.add(player)

    score = 0
    enemy_speed_range = (3, 6)
    spawn_enemy_interval = 1500

    SPAWN_COIN = pygame.USEREVENT + 1
    SPAWN_ENEMY = pygame.USEREVENT + 2
    pygame.time.set_timer(SPAWN_COIN, 1000)
    pygame.time.set_timer(SPAWN_ENEMY, spawn_enemy_interval)

    playing = True
    while playing:
        clock.tick(FPS)
        pressed = pygame.key.get_pressed()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return False
            if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                return False
            if event.type == SPAWN_COIN:
                coin = Coin()
                all_sprites.add(coin)
                coins.add(coin)
            if event.type == SPAWN_ENEMY:
                enemy = Enemy(enemy_speed_range)
                all_sprites.add(enemy)
                enemies.add(enemy)
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                pos = pygame.mouse.get_pos()
                for enemy in enemies:
                    if enemy.rect.collidepoint(pos):
                        enemy.kill()
                        score += 5
                        if collect_sfx:
                            collect_sfx.play()

        player.update(pressed)
        coins.update()
        enemies.update()

        # колизия игрока и монет
        for _ in pygame.sprite.spritecollide(player, coins, True):
            score += 1
            if collect_sfx:
                collect_sfx.play()

        # колизия игрока и врагов
        hits = pygame.sprite.spritecollide(player, enemies, True)
        if hits:
            player.health -= len(hits)
            if hit_sfx:
                hit_sfx.play()

        # измменение сложности
        new_min, new_max, new_interval = get_enemy_parameters(score)
        if (new_min, new_max) != enemy_speed_range:
            enemy_speed_range = (new_min, new_max)
        if new_interval != spawn_enemy_interval:
            spawn_enemy_interval = new_interval
            pygame.time.set_timer(SPAWN_ENEMY, spawn_enemy_interval)

        if player.health <= 0:
            playing = False
            break

        screen.fill((30, 30, 45))
        all_sprites.draw(screen)

        screen.blit(FONT.render(f"Счёт: {score}", True, (255, 255, 255)), (10, 10))
        screen.blit(FONT.render(f"Здоровье: {player.health}", True, (255, 255, 255)), (10, 40))

        pygame.display.flip()

    return game_over_screen(score)


def main():
    while True:
        if not run_game():
            break
    pygame.quit()
    sys.exit()


if __name__ == "__main__":
    main()
