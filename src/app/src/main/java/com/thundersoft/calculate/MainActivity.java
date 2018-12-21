package com.thundersoft.calculate;

import android.content.ClipboardManager;
import android.content.Context;
import android.os.Bundle;
import android.support.design.widget.Snackbar;
import android.text.Editable;
import android.text.Spannable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.text.style.ForegroundColorSpan;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.GridView;
import android.widget.TextView;



import java.util.Arrays;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class MainActivity extends BaseActivity {

    public static MainActivity activity;
    private Context context;
    private EditText inText;
    private TextView stateText;
    private TextView outText;
    public FrameLayout delete;

    private static final int[] XX = {1, 3, 1, 3};
    private static final int[] YY = {6, 4, 5, 5};

    private static final String[] OPERATOR = {"÷", "×", "-", "+", "!"};
    private static final String[] OPERATOR_VICE = {"√", "^", "()", "[", "]"};


    private static final String[] NUMERIC = {
            "7", "8", "9",
            "4", "5", "6",
            "1", "2", "3",
            "·", "0", "=",
            "a", "b", "c",
            "d", "l", "m",
            "n", "x", "y",
            "z", "pi", "exp"};
    private static final Pattern KEYWORDS1 = Pattern.compile("[\\]\\[]");
    private static final Pattern KEYWORDS2 = Pattern.compile("[abcdlmnxyzπe]");
    private static final Pattern CHECK_STRING = Pattern.compile("[//++\\-\\-**√√^^!!\\[\\[]{2,}");


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        activity = this;
        context = this;
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initEditText();
        initTextView();
        initDelete();
        initNumeric();
        initOperator();
    }


    private void initDelete() {
        delete = (FrameLayout) findViewById(R.id.delete);
        delete.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Editable editable = inText.getText();
                int index = inText.getSelectionStart();
                int index2 = inText.getSelectionEnd();
                if (index == index2) {
                    if (index == 0) return;
                    editable.delete(index - 1, index);
                } else {
                    editable.delete(index, index2);
                }
            }
        });
        delete.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View v) {
                inText.setText(null);
                return true;
            }
        });
    }

    private void initTextView() {
        stateText = (TextView) findViewById(R.id.text_state);
        stateText.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stateText.setText(null);
            }
        });
        outText = (TextView) findViewById(R.id.text_out);
        outText.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ClipboardManager cmb = (ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);
                cmb.setText(value);
                Snackbar.make(v, "已复制运算结果", Snackbar.LENGTH_SHORT).show();
            }
        });
        outText.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View v) {
                ResultsActivity.actionStart(v.getContext(), value);
                return true;
            }
        });
    }


    private void initNumeric() {
        GridView numericBar = (GridView) findViewById(R.id.bar_numeric);
        numericBar.setNumColumns(XX[1]);
        numericBar.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                //String str = position == 9 ? "." : NUMERIC[position];
                //change pi = π exp = e;
                String str;
                if (position == 9) {
                    str = ".";
                }/*  else if (position == 22) {
                    str = "π";
                } else if (position == 23) {
                    str = "e";
                }*/ else {
                    str = NUMERIC[position];
                }

                if (str.equals("=")) {
                    checkLastStr(inText.getText().toString());
                    if (calcThread != null) {
                        Snackbar.make(view, "请等待当前运算完成", Snackbar.LENGTH_SHORT)
                                .setAction("停止运算", new View.OnClickListener() {
                                    @Override
                                    public void onClick(View v) {
                                    }
                                }).show();
                        return;
                    }
                    outText.setTextColor(0xffbdbdbd);
                    stateText.setText("运算中...");
                    calcThread = new Calc(inText.getText().toString()+"=");
                    Log.d("wenjie","shuru: "+inText.getText().toString()+"=");
                    calcThread.start();
                    return;
                }
                modifyInText(str);
            }
        });
        GridViewAdapter numericAdapter = new GridViewAdapter(numericBar, Arrays.asList(NUMERIC),
                null, R.layout.button_numeric, YY[1]);
        numericBar.setAdapter(numericAdapter);
    }
    public void checkLastStr(String s){

        char c = s.charAt(s.length()-1);

        //Pattern str = Pattern.compile("[+\\-*/√^]");
        Pattern str = Pattern.compile("\\W");

        for (Matcher m = str.matcher(c+""); m.find(); )
            outText.setText("输入有误,请重新输入!");
        return;


    }

    private void initOperator() {
        GridView operatorBar = (GridView) findViewById(R.id.bar_operator);
        operatorBar.setNumColumns(XX[2]);
        operatorBar.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String str = position == 0 ? "/" : OPERATOR[position];
                modifyInText(position == 1 ? "*" : str);
            }
        });
        operatorBar.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id) {
                modifyInText(OPERATOR_VICE[position]);
                return true;
            }
        });
        GridViewAdapter operatorAdapter = new GridViewAdapter(operatorBar, Arrays.asList(OPERATOR),
                Arrays.asList(OPERATOR_VICE), R.layout.button_operator, YY[2]);
        operatorBar.setAdapter(operatorAdapter);
    }


    private void modifyInText(String str) {
        int index = inText.getSelectionStart();
        int index2 = inText.getSelectionEnd();
        if (index == index2) {
            inText.getText().insert(index, str);
        } else {
            inText.getText().replace(index, index2, str);
        }
    }

    class FastCalc extends Thread implements Runnable {
        private String exp;

        public FastCalc(String exp) {
            this.exp = exp;
        }

        @Override
        public void run() {
            final long t = System.currentTimeMillis();
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    outText.setTextColor(0xffbdbdbd);
                    calcThread = null;
                }
            });
        }
    }

    class Calc extends Thread implements Runnable {
        private String exp;

        public Calc(String exp) {
            this.exp = exp;
        }

        @Override
        public void run() {
            final long t = System.currentTimeMillis();

            value = new SimpleExpression().sendExpressionStr(exp);
            Log.d("wenjie","shuchu :"+value);
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    stateText.setText("");
                    calcThread = null;
                    Pattern str = Pattern.compile("[abcdmnxyzl]{1,}");
                   for (Matcher m = str.matcher(value+""); m.find(); ){
                       Log.d("wenjie","value contain zimu");
                       outText.setText(value+"  --数值包含字母,请长按此处显示结果!");
                       outText.setTextSize(22);
                       return;
                   }
                    outText.setText(value);

                }
            });
        }

    }

    private boolean modified = true;
    private int selection = 0;
    private Thread calcThread;
    private String value;

    private void initEditText() {
        inText = (EditText) findViewById(R.id.editText);
        AutofitHelper.create(inText).setMinTextSize(28).setMaxLines(1);
        inText.requestFocus();
        inText.requestFocusFromTouch();
        inText.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                if (TextUtils.isEmpty(s)) {
                    if (calcThread == null)
                        stateText.setText(null);
                    outText.setTextColor(0xffbdbdbd);
                    outText.setText(null);
                    value = null;
                    return;
                }
                //确保第一个是数字或者字母
                checkFirstStr(s);
                //确保输入计算式正确
                checkStr(s);


                if (calcThread == null) {
                    stateText.setText("运算中...");
                    calcThread = new FastCalc(s.toString());
                    calcThread.start();
                }
            }

            @Override
            public void afterTextChanged(Editable s) {
                if (!modified) return;

                //确保表达式正确,删除提示.(无语,)
                if(!s.toString().contains("//")){
                    outText.setText("");
                }

                selection = inText.getSelectionStart();
                s.clearSpans();

                for (Matcher m = Pattern.compile("[()\\-*+.,/!^=√•]").matcher(s); m.find(); )
                    s.setSpan(new ForegroundColorSpan(0xff81d4fa), m.start(), m.end(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                for (Matcher m = KEYWORDS1.matcher(s); m.find(); )
                    s.setSpan(new ForegroundColorSpan(0xff81d4fa), m.start(), m.end(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                for (Matcher m = KEYWORDS2.matcher(s); m.find(); )
                    s.setSpan(new ForegroundColorSpan(0xfffff59d), m.start(), m.end(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
                modified = false;
                inText.setText(s);
                modified = true;

                if (selection >= 2 && s.toString().substring(selection - 2, selection).equals("()"))
                    selection--;
                inText.setSelection(selection);
            }
        });
    }
    public void checkFirstStr(CharSequence s){
        char c = s.toString().charAt(0);

        Pattern str = Pattern.compile("\\W");

        for (Matcher m = str.matcher(c+""); m.find(); )
            outText.setText("输入有误,请重新输入!");
        return;



    }
    public void checkStr(CharSequence s){

        for (Matcher m = CHECK_STRING.matcher(s); m.find(); )
            outText.setText("输入有误,请重新输入!");

        return;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {

        menu.add("关于").setOnMenuItemClickListener(new MenuItem.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem item) {
                AboutActivity.actionStart(context);
                return true;
            }
        });


        menu.add("退出").setOnMenuItemClickListener(new MenuItem.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem item) {
                finish();
                return true;
            }
        });
        return true;
    }


    static {
        System.loadLibrary("MyJni");
    }

}

