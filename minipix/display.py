import pygame, sys, random
from pygame.locals import *
from time import sleep
from clustercount import PmfFile, cluster_count

arr = []

RED = (255, 10, 10)
BLU = (10, 255, 10)
GRN = (10, 10, 255)
WHITE = (255, 255, 255)

arr.append(RED)
arr.append(BLU)
arr.append(GRN)

SCREEN_X = 256
SCREEN_Y = 256
threshold = 0

pygame.init()

screen = pygame.display.set_mode((512, 512))
square = pygame.Surface((2, 2))
myfont = pygame.font.SysFont("monospace", 15)


def do_visualization(file, frame):
    data = file.get_frame(frame)
    cluster_info = cluster_count(file, data)

    for j in range(SCREEN_Y):
        for i in range(SCREEN_X):
            if data[j][i].value > int(sys.argv[3]):
                num = 1
            else:
                num = 2
            square.fill(arr[num])
            draw_me = pygame.Rect((j + 1) * 2, (i + 1) * 2, 2, 2)
            screen.blit(square, draw_me)
    pygame.display.flip()

    for cluster in cluster_info:
        y, x, _ = cluster[4]
        pixel = pygame.Surface((2, 2))
        #label = myfont.render(str(round(cluster[2][2], 2)), 1, (255, 255, 0))
        pixel.fill(WHITE)
        draw = pygame.Rect(x * 2, y * 2, 2, 2)

        #pygame.draw.line(screen, RED, (x * 2, 0), (x * 2, 512))
        #pygame.draw.line(screen, RED, (0, y * 2), (512, y * 2))
        #screen.blit(label, ((x * 2) + 5, (y * 2) + 5))
        screen.blit(pixel, draw)
    pygame.display.flip()

    return len(cluster_info)


done = False
frame = int(sys.argv[2])
file = PmfFile(sys.argv[1])
file.load_calib_a("a.txt")
file.load_calib_b("b.txt")
file.load_calib_c("c.txt")
file.load_calib_t("t.txt")

clusters = do_visualization(file, frame)

while not done:
    frame_label = myfont.render("Frame {} of {} Clusters: {}".format(frame, file.num_frames-1, clusters), 1, (255, 255, 0))

    screen.blit(frame_label, (5, 0))
    pygame.display.flip()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                if frame > 0:
                    frame -= 1
                clusters = do_visualization(file, frame)
            if event.key == pygame.K_RIGHT:
                if frame < file.num_frames-1:
                    frame += 1
                clusters = do_visualization(file, frame)
