import numpy as np
import cv2


class Rectangle():
    
    
    def __init__(self, frame, x, y, rect_size, password_color):
        
        self.frame = frame
        self.rect_size = rect_size
        self.start_point = (int(x - rect_size/2), int(y - rect_size/2))
        self.end_point = (int(x + rect_size/2), int(y + rect_size/2))
        self.set_color(password_color)
        self.x = x
        self.y = y

        self.org = self.end_point
        self.font = cv2.FONT_HERSHEY_SIMPLEX
        self.fontScale = 0.7
        h_sensivity = 20
        s_h = 255
        v_h = 255
        s_l = 50
        v_l = 50
        self.lowers = []
        self.uppers = []
        #red1, red2, green, blue
        color_point = [0, 180, 60, 120]
        for i in range(4):
            lower_point = color_point[i] - h_sensivity if color_point[i] - h_sensivity >= 0 else 0
            self.lowers.append(np.array([lower_point, s_l, v_l]))
            upper_point = color_point[i] + h_sensivity if color_point[i] + h_sensivity <= 180 else 180
            self.uppers.append(np.array([upper_point, s_h, v_h]))
        self.right_color = None
    
    def is_color(self):
        hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        self.mask_frame = hsv_frame[self.start_point[1]:self.end_point[1] + 1, self.start_point[0]:self.end_point[0] + 1]
        #red1, red2, green, blue
        color_masks = []
        rates = []
        for i in range(4):
            color_masks.append(cv2.inRange(self.mask_frame, self.lowers[i], self.uppers[i]))
            rates.append(np.count_nonzero(color_masks[i])/(self.rect_size**2))
        color_now = "None"
        min_color_rate = 0.9
        #red
        if rates[0]+rates[1] > min_color_rate:
            color_now = "red"
        #green
        elif rates[2] > min_color_rate:
            color_now = "green"
        #blue
        elif rates[3] > min_color_rate:
            color_now = "blue"
            
        cv2.putText(self.frame, " "+color_now+" " , (int(self.x - self.rect_size/2), int(self.y + self.rect_size/2 + 20)), self.font, self.fontScale, (255, 0, 0), 2, cv2.LINE_AA)
        if color_now == self.color:
            self.right_color =  True
        else:
            self.right_color = False
        return self.right_color
            
    
    def set_color(self, color):
        if (color in ["red", "green", "blue"]):
            self.color = color
        else:
            self.color = "Error"

    def draw(self):
        if self.right_color ==  None:
            self.is_color()
        if self.right_color == True:
            rect_color = (0, 255, 0)
        else:
            rect_color = (255, 0, 0)
        self.rect = cv2.rectangle(self.frame, self.start_point, self.end_point, rect_color, 2)
        
    
        
def process(frame):
    rect_size = 100
    height, width, channels = frame.shape
    password = ["red", "red", "red", "red"]
    correct_password = [False]*4
    lock = "Locked"
    rects = []

    for i in range(4):
        rects.append(Rectangle( frame, int(width/4 * i + width/8),int(height/2), rect_size, password[i]))
        correct_password[i] = rects[i].is_color() 
        rects[i].draw()
    
    for i in range(4):
        if correct_password[i] == False:
            break
    else:
        lock = "Unlocked"
    
    """
    h_sensivity = 20
    s_h = 255
    v_h = 255
    s_l = 50
    v_l = 50
    width, height, channels = frame.shape
    start_point = (int(height/2 - rect_size/2), int(width/2 - rect_size/2))
    end_point = (int(height/2 + rect_size/2), int(width/2 + rect_size/2))
    color = (255, 0, 0)
    thickness = 2
    rect = cv2.rectangle(frame, start_point, end_point, color, thickness)

    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    green_upper = np.array([60 + h_sensivity, s_h, v_h])
    green_lower = np.array([60 - h_sensivity, s_l, v_l])
    mask_frame = hsv_frame[start_point[1]:end_point[1] + 1, start_point[0]:end_point[0] + 1]
    mask_green = cv2.inRange(mask_frame, green_lower, green_upper)

    green_rate = np.count_nonzero(mask_green)/(rect_size*rect_size)

    if green_rate > 0.9:
        text = cv2.putText(rect, ' green ', org, font, fontScale, color, thickness, cv2.LINE_AA)
    else:
        text = cv2.putText(rect, ' not green ', org, font, fontScale, color, thickness, cv2.LINE_AA)

    av_hue = np.average(mask_frame[:,:,0])
    av_sat = np.average(mask_frame[:,:,1])
    av_val = np.average(mask_frame[:,:,2])
    average = [int(av_hue),int(av_sat),int(av_val)]
    """
    font = cv2.FONT_HERSHEY_SIMPLEX
    fontScale = 0.7
    
    text = cv2.putText(frame, lock,\
                       (10,50), font, fontScale, (255, 0, 0), 2, cv2.LINE_AA)
    frame = text
    return frame
    

print('Press 4 to Quit the Application\n')

#Open Default Camera
cap = cv2.VideoCapture(0)#gstreamer_pipeline(flip_method=4), cv2.CAP_GSTREAMER)

while(cap.isOpened()):
    #Take each Frame
    ret, frame = cap.read()
    
    #Flip Video vertically (180 Degrees)
    frame = cv2.flip(frame, 180)

    invert = process(frame)

    # Show video
    # cv2.imshow('Cam', frame)
    cv2.imshow('Inverted', invert)

    # Exit if "4" is pressed
    k = cv2.waitKey(1) & 0xFF
    if k == 52 : #ord 4
        #Quit
        print ('Good Bye!')
        break

#Release the Cap and Video   
cap.release()
cv2.destroyAllWindows()