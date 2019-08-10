import tkinter as tk
from tkinter import font  as tkfont
#import pyqrcode as QR


class CocktailGUI(tk.Tk):

    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        self.title_font = tkfont.Font(family='Comic Sans MS', size=18, weight="bold", slant="italic")
        self.geometry('480x320')
        container = tk.Frame(self)
        container.pack(side="top", fill="both", expand=True)
        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)

        self.frames = {}
        for F in (StartPage, PWPage, AdminPage, InitPage, MenuPage, AnmeldePage, QRPage, AuswahlPage, MixPage):
            page_name = F.__name__
            frame = F(parent=container, controller=self)
            self.frames[page_name] = frame

            frame.grid(row=0, column=0, sticky="nsew")

        self.show_frame("MenuPage")

    def show_frame(self, page_name):
        frame = self.frames[page_name]
        frame.tkraise()


class StartPage(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        label = tk.Label(self, text="StartPage", font=controller.title_font)
        label.place(relx=0.2,relwidth=0.6)

        button1 = tk.Button(self, text="Go to Page One",
                            command=lambda: controller.show_frame("PageOne"))
        button2 = tk.Button(self, text="Go to Page Two",
                            command=lambda: controller.show_frame("PageTwo"))

        button3 = tk.Button(self, text="Go to Page Three",
                            command=lambda: controller.show_frame("PageOne"))
        button4 = tk.Button(self, text="Go to Page Four",
                            command=lambda: controller.show_frame("PageTwo"))
        button1.place(relx=0.0,rely=0.0, relwidth=0.2, relheight=0.5)
        button2.place(relx=0,rely=0.5, relwidth=0.2, relheight=0.5)
        button3.place(relx=0.8,rely=0, relwidth=0.2, relheight=0.5)
        button4.place(relx=0.8,rely=0.5, relwidth=0.2, relheight=0.5)

class PWPage(tk.Frame):
    
    def pwCheck(self, code):
        self.pweingabe += code
        if(len(self.pweingabe)==4):
            if(self.pweingabe=="4321"):
                self.pweingabe=""
                self.controller.show_frame("AdminPage")
            else:
                self.pweingabe=""
                self.controller.show_frame("MenuPage")
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        self.pweingabe =""
        label = tk.Label(self, text="PWPage", font=controller.title_font)
        label.place(relx=0.2,relwidth=0.6)
        button1 = tk.Button(self, text="1",
                            command=lambda: self.pwCheck("1"), bg="turquoise2")
        button2 = tk.Button(self, text="2",
                            command=lambda: self.pwCheck("2"), bg="turquoise2")

        button3 = tk.Button(self, text="3",
                            command=lambda: self.pwCheck("3"), bg="turquoise2")
        button4 = tk.Button(self, text="4",
                            command=lambda: self.pwCheck("4"), bg="turquoise2")
        button1.place(relx=0.0,rely=0.0, relwidth=0.2, relheight=0.5)
        button2.place(relx=0.8,rely=0, relwidth=0.2, relheight=0.5)
        button3.place(relx=0,rely=0.5, relwidth=0.2, relheight=0.5)
        button4.place(relx=0.8,rely=0.5, relwidth=0.2, relheight=0.5)

class AdminPage(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        label = tk.Label(self, text="AdminPage", font=controller.title_font)
        label.place(relx=0.2,relwidth=0.6)

        button1 = tk.Button(self, text="Cancel",
                            command=lambda: controller.show_frame("MenuPage"), bg="turquoise2")
        button2 = tk.Button(self, text="Submit", bg="turquoise2")
        button3 = tk.Button(self, text="Up", bg="turquoise2")
        button4 = tk.Button(self, text="Down", bg="turquoise2")
        button1.place(relx=0.0,rely=0.0, relwidth=0.2, relheight=0.5)
        button2.place(relx=0,rely=0.5, relwidth=0.2, relheight=0.5)
        button3.place(relx=0.8,rely=0, relwidth=0.2, relheight=0.5)
        button4.place(relx=0.8,rely=0.5, relwidth=0.2, relheight=0.5)

class InitPage(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        label = tk.Label(self, text="InitPage", font=controller.title_font)
        label.place(relx=0.2,relwidth=0.6)

        button1 = tk.Button(self, text="Cancel",
                            command=lambda: controller.show_frame("AdminPage"), bg="turquoise2")
        button2 = tk.Button(self, text="Submit",
                            command=lambda: controller.show_frame("MenuPage"), bg="turquoise2")
        button3 = tk.Button(self, text="")
        button4 = tk.Button(self, text="")
        button1.place(relx=0.0,rely=0.0, relwidth=0.2, relheight=0.5)
        button2.place(relx=0,rely=0.5, relwidth=0.2, relheight=0.5)
        button3.place(relx=0.8,rely=0, relwidth=0.2, relheight=0.5)
        button4.place(relx=0.8,rely=0.5, relwidth=0.2, relheight=0.5)

class MenuPage(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        label = tk.Label(self, text="MenuPage", font=controller.title_font)
        label.place(relx=0.2,relwidth=0.6)

        button1 = tk.Button(self, text="Manuelle Auswahl",
                            command=lambda: controller.show_frame("AuswahlPage"), bg="turquoise2")
        button2 = tk.Button(self, text="Admin",
                            command=lambda: controller.show_frame("PWPage"), bg="turquoise2")

        button3 = tk.Button(self, text="QR Scannen",
                            command=lambda: controller.show_frame("QRPage"), bg="turquoise2")
        button4 = tk.Button(self, text="Handy anmelden",
                            command=lambda: controller.show_frame("AnmeldePage"), bg="turquoise2")
        button1.place(relx=0.0,rely=0.0, relwidth=0.2, relheight=0.5)
        button2.place(relx=0,rely=0.5, relwidth=0.2, relheight=0.5)
        button3.place(relx=0.8,rely=0, relwidth=0.2, relheight=0.5)
        button4.place(relx=0.8,rely=0.5, relwidth=0.2, relheight=0.5)

class AnmeldePage(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        label = tk.Label(self, text="AnmeldePage", font=controller.title_font)
        label.place(relx=0.2,relwidth=0.6)

        button1 = tk.Button(self, text="Cancel",
                            command=lambda: controller.show_frame("MenuPage"), bg="turquoise2")
        button2 = tk.Button(self, text="")
        button3 = tk.Button(self, text="")
        button4 = tk.Button(self, text="")
        button1.place(relx=0.0,rely=0.0, relwidth=0.2, relheight=0.5)
        button2.place(relx=0,rely=0.5, relwidth=0.2, relheight=0.5)
        button3.place(relx=0.8,rely=0, relwidth=0.2, relheight=0.5)
        button4.place(relx=0.8,rely=0.5, relwidth=0.2, relheight=0.5)

class QRPage(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        label = tk.Label(self, text="QRPage", font=controller.title_font)
        label.place(relx=0.2,relwidth=0.6)

        button1 = tk.Button(self, text="Cancel",
                            command=lambda: controller.show_frame("MenuPage"), bg="turquoise2")
        button2 = tk.Button(self, text="")
        button3 = tk.Button(self, text="")
        button4 = tk.Button(self, text="")
        button1.place(relx=0.0,rely=0.0, relwidth=0.2, relheight=0.5)
        button2.place(relx=0,rely=0.5, relwidth=0.2, relheight=0.5)
        button3.place(relx=0.8,rely=0, relwidth=0.2, relheight=0.5)
        button4.place(relx=0.8,rely=0.5, relwidth=0.2, relheight=0.5)

class AuswahlPage(tk.Frame):
    def showListElement(self,direction):
        self.listboxCocktails.selection_clear(0,self.listboxCocktails.size()-1)
        if(direction=="DOWN"):
            self.activeElement = (self.activeElement+1)%self.listboxCocktails.size()
            self.listboxCocktails.activate(self.activeElement)
            self.listboxCocktails.selection_set(self.activeElement)
            self.listboxCocktails.see(self.activeElement)
        elif(direction=="UP"):
            self.activeElement = (self.activeElement-1)%self.listboxCocktails.size()
            self.listboxCocktails.activate(self.activeElement)
            self.listboxCocktails.selection_set(self.activeElement)
            self.listboxCocktails.see(self.activeElement)
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        self.activeElement = 0
        label = tk.Label(self, text="AuswahlPage", font=controller.title_font)
        label.place(relx=0.2,relwidth=0.6)

        button1 = tk.Button(self, text="Cancel",
                            command=lambda: controller.show_frame("MenuPage"), bg="turquoise2")
        button2 = tk.Button(self, text="OK",
                            command=lambda: controller.show_frame("MixPage"), bg="turquoise2")
        button3 = tk.Button(self, text="Up", command=lambda: self.showListElement("UP"), bg="turquoise2")
        button4 = tk.Button(self, text="Down", command=lambda: self.showListElement("DOWN"), bg="turquoise2")
        button1.place(relx=0.0,rely=0.0, relwidth=0.2, relheight=0.5)
        button2.place(relx=0,rely=0.5, relwidth=0.2, relheight=0.5)
        button3.place(relx=0.8,rely=0, relwidth=0.2, relheight=0.5)
        button4.place(relx=0.8,rely=0.5, relwidth=0.2, relheight=0.5)

        self.listboxCocktails = tk.Listbox(self, selectmode='browse', background="Black", fg="turquoise",selectbackground="Green",highlightcolor="Pink", font = ('Comic Sans MS',20))
        self.listboxCocktails.insert('end', 'Caipi')
        self.listboxCocktails.insert('end', 'Banane-Kirsch')
        self.listboxCocktails.insert('end', 'Vodkabull')
        self.listboxCocktails.insert('end', 'Dominik')
        self.listboxCocktails.place(relx=0.2, y=0, relwidth=0.6, relheight=1)
        self.listboxCocktails.selection_set(0)
        self.listboxCocktails.activate(0)

class MixPage(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        label = tk.Label(self, text="Glas platzieren und <Start> drücken", font=controller.title_font)
        label.place(relx=0.2,relwidth=0.6)

        button1 = tk.Button(self, text="Cancel",
                            command=lambda: controller.show_frame("MenuPage"), bg="turquoise2")
        button2 = tk.Button(self, text="Start", bg="turquoise2")
        button3 = tk.Button(self, text="")
        button4 = tk.Button(self, text="")
        button1.place(relx=0.0,rely=0.0, relwidth=0.2, relheight=0.5)
        button2.place(relx=0,rely=0.5, relwidth=0.2, relheight=0.5)
        button3.place(relx=0.8,rely=0, relwidth=0.2, relheight=0.5)
        button4.place(relx=0.8,rely=0.5, relwidth=0.2, relheight=0.5)


if __name__ == "__main__":
    app = CocktailGUI()
    app.mainloop()
