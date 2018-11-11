package log

import (
	"sync"
	"fmt"
	"common/terminal"
	"time"
	"os"
	"runtime"
)

// type define
type formatCB func(level int, fmt string, v... interface{}) string

type Writer interface {
	Name() string
	Write(p []byte) (n int, err error)
	Close() error
}

// cross-process locks to ensure complete writes.
type Lock interface {
	Lock()
	Unlock()
}

type logEntry struct {
	writer	Writer
	color	int
	format	string
	v		[]interface{}
}

type queue struct {
	enable	bool
	isStop	chan bool
	buf		chan logEntry
}

type Logger struct{
	level	int					// logger level.
	out 	[]Writer			// destination for output.
	mutex	sync.Mutex			// ensures atomic writes; protects the following fields.
	lock	Lock				// cross-process locks to ensure complete writes.
	format	formatCB			// user format string callback.
	color	bool				// false: only level name with color, true: then all with color.
	queue	queue				// buffer queue,support asynchronous processing.
}

// log level
const (
	ALL = iota
	TRACE
	DEBUG
	INFO
	WARNING
	ERROR
	FATAL
	OFF
)
var levelName  = map[int]string{
	TRACE         : "TRAC",
	DEBUG         : "DEBU",
	INFO          : "INFO",
	WARNING       : "WARN",
	ERROR         : "ERRO",
	FATAL         : "FATA",
}
var levelColor = map[int]int{
	TRACE         : terminal.Cyan,
	DEBUG         : terminal.Green,
	INFO          : terminal.Blue,
	WARNING       : terminal.Purple,
	ERROR         : terminal.Red,
	FATAL         : terminal.Red,
}


// default
const defaultLevel = TRACE
const defaultColor = false
var defaultWriter = os.Stderr
func defaultLineBreak() string {
	switch runtime.GOOS {
	case "windows":
		return "\r\n"
	default:
		return "\n"
	}

	return "\n"
}
func defaultFormatCB(level int, format string, v... interface{}) string {
	return time.Now().Format(time.RFC3339) + " " + fmt.Sprintf(format, v...)
}

// create logger.
func New(cb formatCB) *Logger{
	log := Logger{}

	switch cb {
	case nil:
		cb = defaultFormatCB
	}

	log.out = append(log.out, defaultWriter)
	log.level = defaultLevel
	log.lock = nil
	log.format = cb
	log.color = defaultColor
	log.queue.buf = make(chan logEntry, 1024)
	log.queue.enable = false
	log.queue.isStop = make(chan bool)

	return &log
}

// output log to writers.
func (log *Logger) writeLog(color int, format string, v... interface{}){
	for _,w := range log.out {
		if log.queue.enable {
			log.queue.buf <- logEntry{writer:w, color:color, format:format, v:v}
		} else {
			terminal.ColorWrite(w, color, format, v...)
		}
	}
}

// mutex sync, output log to writer.
func (log *Logger) logOut(level int, format string, v... interface{}) {
	if log.level > level {
		return
	}

	log.mutex.Lock()
	defer log.mutex.Unlock()

	if log.lock != nil {
		log.lock.Lock()
		defer log.lock.Unlock()
	}

	lName := levelName[level]
	lColor := levelColor[level]

	var fmtstr string
	if log.format != nil {
		fmtstr = log.format(level, format, v...)
	} else {
		fmtstr = fmt.Sprintf("%s", v...)
	}

	fmtstr += defaultLineBreak()

	log.writeLog(lColor, lName + " ")

	if !log.color {
		lColor = terminal.DefaultColor
	}

	log.writeLog(lColor, fmtstr)
}



/* user configure interface */

// add writer.
func (log *Logger) AddWriters(writers ... Writer) {
	log.mutex.Lock()
	defer log.mutex.Unlock()

	log.out = append(log.out, writers...)
}

// remove writer.
func (log *Logger) RemoveWriters(writers ...Writer) {
	log.mutex.Lock()
	defer log.mutex.Unlock()

	out := log.out
	log.out = nil

	for _,w := range out {
		flag := true
		for _,rw := range writers {
			if rw == w {
				flag = false
				break
			}
		}

		if flag {
			log.out = append(log.out, w)
		}
	}
}

// set logger level.
func (log *Logger) SetLevel(level int){
	log.mutex.Lock()
	defer log.mutex.Unlock()

	log.level = level
}

// set logger level.
func (log *Logger) GetLevel() int {
	log.mutex.Lock()
	defer log.mutex.Unlock()

	return log.level
}

// set lock.
func (log *Logger) SetLock(lock Lock) {
	log.mutex.Lock()
	defer log.mutex.Unlock()

	log.lock = lock
}

// get lock.
func (log *Logger) GetLock() Lock {
	log.mutex.Lock()
	defer log.mutex.Unlock()

	return log.lock
}

// set display color.
func (log *Logger) SetColor(flag bool) {
	log.mutex.Lock()
	defer log.mutex.Unlock()

	log.color = flag
}

// get display status.
func (log *Logger) GetColor() bool {
	log.mutex.Lock()
	defer log.mutex.Unlock()

	return log.color
}

// set format callback.
func (log *Logger) SetFormatCB(cb formatCB) {
	log.mutex.Lock()
	defer log.mutex.Unlock()

	log.format = cb
}

// get format callback.
func (log *Logger) GetFormatCB() formatCB {
	log.mutex.Lock()
	defer log.mutex.Unlock()

	return log.format
}

// set enable mq
func (log *Logger) SetEnableMQ(flag bool) {
	log.mutex.Lock()
	defer log.mutex.Unlock()

	if flag {
		go func(){
			end:
			for {
				select {
				case entry := <- log.queue.buf:
					terminal.ColorWrite(entry.writer, entry.color, "queue: " + entry.format, entry.v...)
				default:
					select {
					case <- log.queue.isStop:
						fmt.Println("is break=========================================")
						break end
					}
				}
			}
			fmt.Println("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
		}()
	} else {
		log.queue.isStop <- true
	}

	log.queue.enable = flag
}

// get enable mq
func (log *Logger) GetEnableMQ() bool {
	log.mutex.Lock()
	defer log.mutex.Unlock()

	return log.queue.enable
}

/* level out log */

func (log *Logger) BlankLine() {
	log.mutex.Lock()
	defer log.mutex.Unlock()

	log.writeLog(terminal.DefaultColor, defaultLineBreak())
}

func (log *Logger) Trace(format string, v... interface{}){
	log.logOut(TRACE, format, v...)
}

func (log *Logger) Debug(format string, v... interface{}){
	log.logOut(DEBUG, format, v...)
}

func (log *Logger) Info(format string, v... interface{}){
	log.logOut(INFO, format, v...)
}

func (log *Logger) Warning(format string, v... interface{}){
	log.logOut(WARNING, format, v...)
}

func (log *Logger) Error(format string, v... interface{}){
	log.logOut(ERROR, format, v...)
}

func (log *Logger) Fatal(format string, v... interface{}){
	log.logOut(FATAL, format, v...)
}

func (log *Logger) B() {
	log.BlankLine()
}

func (log *Logger) T(format string, v... interface{}) {
	log.logOut(TRACE, format, v...)
}

func (log *Logger) D(format string, v... interface{}){
	log.logOut(DEBUG, format, v...)
}

func (log *Logger) I(format string, v... interface{}){
	log.logOut(INFO, format, v...)
}

func (log *Logger) W(format string, v... interface{}){
	log.logOut(WARNING, format, v...)
}

func (log *Logger) E(format string, v... interface{}){
	log.logOut(ERROR, format, v...)
}

func (log *Logger) F(format string, v... interface{}){
	log.logOut(FATAL, format, v...)
}
