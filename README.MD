```plantuml
title mmap-writer 类图
header www.salton123.com


class MmapWriter{

}

class Metadata {

}

class FileAsyncFlusher {

}

class BufferTrigger{

}

class FlushBuffer{

}


Metadata --> BufferTrigger
FlushBuffer --> BufferTrigger
BufferTrigger --> MmapWriter
FileAsyncFlusher --> BufferTrigger
```

