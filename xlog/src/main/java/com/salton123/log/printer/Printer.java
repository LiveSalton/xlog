
package com.salton123.log.printer;

/**
 * User: newSalton@outlook.com
 * Date: 2018/11/13 11:28 AM
 * ModifyTime: 11:28 AM
 * Description:we can make more impl ,such as ConsolePrinter,RemotePrinter,ConsolePrinter
 */
public interface Printer {
    void println(int logLevel, String tag, String msg);
}
