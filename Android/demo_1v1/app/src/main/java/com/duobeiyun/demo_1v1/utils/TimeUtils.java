package com.duobeiyun.demo_1v1.utils;

import java.util.Locale;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class TimeUtils {
    public static String long2TimeString(long time) {
        if (time < 0) {
            return String.format(Locale.CHINA, "%02d:%02d:%02d", 0, 0, 0);
        }
        long second = (time / 1000) % 60;
        long minute = (time / (1000 * 60)) % 60;
        long hour = (time / (1000 * 60 * 60)) % 24;
        return String.format(Locale.CHINA, "%02d:%02d:%02d", hour, minute, second);
    }

    /**
     * 将 "02:10:23"格式的时间转换为long类型的时间
     *
     * @param timestr
     * @return
     */
    public static long stringTime2long(String timestr) {
        if (!valiDateTimeWithLongFormat(timestr)) {
            throw new RuntimeException("你传入的课程时间格式不对");
        }
        String[] split = timestr.split(":");
        int hour = Integer.parseInt(split[0]);
        int min = Integer.parseInt(split[1]);
        int sec = Integer.parseInt(split[2]);
        long i = hour * 60 * 60 * 1000;
        long i1 = min * 60 * 1000;
        long i2 = sec * 1000;
        return i + i1 + i2;
    }

    private static boolean valiDateTimeWithLongFormat(String timeStr) {
        String format = "[0-9][0-9]:[0-5][0-9]:[0-5][0-9]";//课程的最大时间不能操作99:59:59
        Pattern pattern = Pattern.compile(format);
        Matcher matcher = pattern.matcher(timeStr);
        return matcher.matches() ? true : false;
    }
}
