#Rahul Amudhasagaran Version 1.0 10/30/2022: Base Program
                    #Version 1.1 12/09/2022: GUI
                    #Version 2.0 3/28/2023: Librosa

from tkinter import *
#import librosa
#from librosa import load as load
#from librosa import beat as beat
#import matplotlib.pyplot as plt

#fig, ax = plt.subplots (nrows = 3, sharex = True)
"""
y, sr = load ('Four Theories.mp3', sr = None)

tempo, beats = beat.beat_track (y = y, sr = sr, units = 'time')

diff = []
for i in range (len (beats) - 1):
    difference = beats [i + 1] - beats [i]
    diff.append (difference)
diff = sorted (diff)
#print (diff)

total = 0
for i in diff:
    total += i

average = total / (len (diff) - 1)
final_temp = 60 / average
act_final_temp = final_temp
#final_temp = round (final_temp * 5) / 5
rounding_error = abs (act_final_temp - final_temp)

print ("Estimated:", tempo, "BPM")
print ("Calculated:", final_temp, "BPM")
print ("Rounding Error:", rounding_error, "BPM")



threshold = average / 20
discarded_values = []
start = end = 0

for i in range (len (diff) - 1):
    if abs (diff [i] - average) < threshold:
        start = i
        break
for i in range (len (diff) - 1):
    if abs (diff [(len (diff) - 1) - i] - average) < threshold:
        end = (len (diff) - 1) - i
        break

diff = diff [start: end]


total = 0
for i in diff:
    total += i

average = total / (len (diff) - 1)
final_temp = 60 / average
act_final_temp = final_temp
#final_temp = round (final_temp * 5) / 5
rounding_error = abs (act_final_temp - final_temp)

print ("Estimated:", tempo, "BPM")
print ("Calculated:", final_temp, "BPM")
print ("Rounding Error:", rounding_error, "BPM")
"""

"""
librosa.display.waveshow(y, sr=sr, ax=ax[0])
ax[0].set(title='Envelope view, mono')
ax[0].label_outer()

plt.show()
"""

"""
import numpy as np
y, sr = librosa.load('HYPE.mp3')
onset_env = librosa.onset.onset_strength(y=y, sr=sr)
pulse = librosa.beat.plp(onset_envelope=onset_env, sr=sr)
# Or compute pulse with an alternate prior, like log-normal
import scipy.stats
prior = scipy.stats.lognorm(loc=np.log(120), scale=120, s=1)
pulse_lognorm = librosa.beat.plp(onset_envelope=onset_env, sr=sr,
                                 prior=prior)
melspec = librosa.feature.melspectrogram(y=y, sr=sr)
import matplotlib.pyplot as plt
fig, ax = plt.subplots(nrows=3, sharex=True)
librosa.display.specshow(librosa.power_to_db(melspec,
                                             ref=np.max),
                         x_axis='time', y_axis='mel', ax=ax[0])
ax[0].set(title='Mel spectrogram')
ax[0].label_outer()
ax[1].plot(librosa.times_like(onset_env),
         librosa.util.normalize(onset_env),
         label='Onset strength')
ax[1].plot(librosa.times_like(pulse),
         librosa.util.normalize(pulse),
         label='Predominant local pulse (PLP)')
ax[1].set(title='Uniform tempo prior [30, 300]')
ax[1].label_outer()
ax[2].plot(librosa.times_like(onset_env),
         librosa.util.normalize(onset_env),
         label='Onset strength')
ax[2].plot(librosa.times_like(pulse_lognorm),
         librosa.util.normalize(pulse_lognorm),
         label='Predominant local pulse (PLP)')
ax[2].set(title='Log-normal tempo prior, mean=120', xlim=[5, 20])
ax[2].legend()
tempo, beats = librosa.beat.beat_track(onset_envelope=onset_env)
beats_plp = np.flatnonzero(librosa.util.localmax(pulse))
fig, ax = plt.subplots(nrows=2, sharex=True, sharey=True)
times = librosa.times_like(onset_env, sr=sr)
ax[0].plot(times, librosa.util.normalize(onset_env),
         label='Onset strength')
ax[0].vlines(times[beats], 0, 1, alpha=0.5, color='r',
           linestyle='--', label='Beats')
ax[0].legend()
ax[0].set(title='librosa.beat.beat_track')
ax[0].label_outer()
# Limit the plot to a 15-second window
times = librosa.times_like(pulse, sr=sr)
ax[1].plot(times, librosa.util.normalize(pulse),
         label='PLP')
ax[1].vlines(times[beats_plp], 0, 1, alpha=0.5, color='r',
           linestyle='--', label='PLP Beats')
ax[1].legend()
ax[1].set(title='librosa.beat.plp', xlim=[5, 20])
ax[1].xaxis.set_major_formatter(librosa.display.TimeFormatter())
plt.show()

avg = 0
for i in range (len (beats_plp) - 1):
    avg += (times [i + 1] - times [i])
avg /= len (beats_plp)
avg *= 128
print (avg)
"""


def funcs (listE, funcNum): #Runs checker and specific function
    funcs = [ml, sc, lm]
    try:
        temp = True
        for e in listE:
            if e.get () == '' or e.get () == txtdc [e]:
                temp = False
        if temp:
            par = [float (e.get ()) for e in listE]
            ansbxs [funcNum] ['text'] = funcs [funcNum] (par)
        else:
            ansbxs [funcNum] ['text'] = 'Please fill out the box(es)!'
    except (ValueError):
        ansbxs [funcNum] ['text'] = 'Wrong Var Type(s)'

def ml (l): #Returns measure length from user given BPM
    bpm = l [0]
    oldbpm, oldml = 128, 15.000
    ml = round ((oldbpm / bpm) * oldml, 3)
    return f'length: {ml}'

def sc (l): #Returns multiplier and percent change from user given speeds  
    oldSp, newSp = l
    mult = round (oldSp / newSp, 5)
    per = round ((mult - 1) * 100, 3)
    return f'mult: {mult}, %: {per}'
   
def lm (l): #Returns new measure length after multiplier
    length, mult = l
    newl = round (length * mult, 3) if not divide.get () else round (length / mult, 3)
    if newl / 60 < 1:
        return f'length: {newl}'
    else:
        minute = int (newl / 60)
        second = round (newl % 60, 3)
        if second > 10:
            return f'length: {int (newl / 60)}:{round (newl % 60, 3)}'
        else:
            return f'length: {int (newl / 60)}:0{round (newl % 60, 3)}'
        
def fcsIn (e): #Deletes temporary text
    if e.get () == txtdc [e]:
        e ['fg'] = 'black'
        e.delete (0, len (e.get ()))

def fcsOut (e, string): #Inserts temporary text
    if e.get () == '':
        e ['fg'] = 'gray'
        e.insert (0, string)

def gui_manager ():
    gui.title ('Audio Analyzer')
    
    topFrame = Frame (gui)
    topFrame.pack (side = TOP)

    bottomFrame = Frame (gui)
    bottomFrame.pack (side = BOTTOM)

    global txtdc
    global ansbxs

    #BPM Converter
    bpmConv = Frame (bottomFrame)
    bpmConv.pack (side = LEFT)

    Label (bpmConv, text = 'BPM Converter').grid (row = 0)

    bg1 = Label (bpmConv)
    bg1.grid (row = 1, pady = 50, padx = 80)

    e1 = Entry (bpmConv, width = 20, borderwidth = 5, fg = 'gray')
    e1.grid (row = 2, column = 0, columnspan = 4, padx = 20, pady = 10)

    e1.bind ('<Return>', lambda ev: funcs ([e1], 0))
    e1.bind ('<FocusIn>', lambda ev: fcsIn (e1))
    e1.bind ('<FocusOut>', lambda ev: fcsOut (e1, txtdc [e1]))

    ab1 = Label (bpmConv)
    ab1.grid (row = 3)

    #Speed Changer
    spdchange = Frame (bottomFrame)
    spdchange.pack (side = LEFT)

    Label (spdchange, text = 'Speed Changer').grid (row = 0)

    bg2 = Label (spdchange)
    bg2.grid (row = 1, pady = 25, padx = 80)

    e2a = Entry (spdchange, width = 20, borderwidth = 5, fg = 'gray')
    e2a.grid (row = 2, column = 0, columnspan = 4, padx = 10, pady = 10)

    e2a.bind ('<Return>', lambda ev: funcs ([e2a, e2b], 1))
    e2a.bind ('<FocusIn>', lambda ev: fcsIn (e2a))
    e2a.bind ('<FocusOut>', lambda ev: fcsOut (e2a, txtdc [e2a]))

    e2b = Entry (spdchange, width = 20, borderwidth = 5, fg = 'gray')
    e2b.grid (row = 3, column = 0, columnspan = 4, padx = 10, pady = 10)

    e2b.bind ('<Return>', lambda ev: funcs ([e2a, e2b], 1))
    e2b.bind ('<FocusIn>', lambda ev: fcsIn (e2b))
    e2b.bind ('<FocusOut>', lambda ev: fcsOut (e2b, txtdc [e2b]))

    ab2 = Label (spdchange)
    ab2.grid (row = 4)

    #Measure Multiplier
    msrmlt = Frame (bottomFrame)
    msrmlt.pack (side = RIGHT)

    Label (msrmlt, text = 'Length Multiplier').grid (row = 0)

    bg3 = Label (msrmlt)
    bg3.grid (row = 1, pady = 5, padx = 80)

    global divide;
    divide = BooleanVar ()

    cb3 = Checkbutton (msrmlt, text = 'Divide', variable = divide, onvalue = True, offvalue = False)
    cb3.grid (row = 2, pady = 5)

    e3a = Entry (msrmlt, width = 20, borderwidth = 5, fg = 'gray')
    e3a.grid (row = 3, column = 0, columnspan = 4, padx = 10, pady = 10)

    e3a.bind ('<Return>', lambda ev: funcs ([e3a, e3b], 2))
    e3a.bind ('<FocusIn>', lambda ev: fcsIn (e3a))
    e3a.bind ('<FocusOut>', lambda ev: fcsOut (e3a, txtdc [e3a]))

    e3b = Entry (msrmlt, width = 20, borderwidth = 5, fg = 'gray')
    e3b.grid (row = 4, column = 0, columnspan = 4, padx = 10, pady = 10)

    e3b.bind ('<Return>', lambda ev: funcs ([e3a, e3b], 2))
    e3b.bind ('<FocusIn>', lambda ev: fcsIn (e3b))
    e3b.bind ('<FocusOut>', lambda ev: fcsOut (e3b, txtdc [e3b]))
    
    ab3 = Label (msrmlt)
    ab3.grid (row = 5)

    #Instantiating
    txtdc = {e1: 'BPM',
             e2a: 'Old Speed',
             e2b: 'New Speed',
             e3a: 'Length',
             e3b: 'Multiplier'}

    ansbxs = [ab1, ab2, ab3]

    ebxs = [e1, e2a, e2b, e3a, e3b]

    for e in ebxs:
        e.insert (0, txtdc [e])

if __name__ == '__main__':
    gui = Tk ()
    gui_manager ()
    gui.mainloop ()